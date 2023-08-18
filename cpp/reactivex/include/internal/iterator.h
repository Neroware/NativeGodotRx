#ifndef RX_ITERATOR_H
#define RX_ITERATOR_H

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "wrapper/iterable.h"
#include "wrapper/iterator.h"
#include "cast.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::wrappers;

namespace rx {

class ItEnd : public RefCounted {
    GDCLASS(ItEnd, RefCounted)

protected:
    static inline void _bind_methods() {}

public:
    ItEnd() {}
    ~ItEnd() {}

    inline String _to_string() const {
	    return "<<ItEnd>>";
    }
};

struct array_iterator : public IteratorBase {
    Array array;
    uint64_t index;
    Ref<ItEnd> itEnd;

    explicit array_iterator(const Array& array_, uint64_t index_ = 0) 
        : array(array_), index(index_), itEnd(memnew(ItEnd)) {}
    
    inline Variant next() override {
        return index >= array.size() ? Variant(this->itEnd) : array[index++];
    }
    inline bool has_next() override {
        return index < array.size();
    }
}; // END array_iterator

struct array_iterable : public IterableBase {
    Array array;

    array_iterable(const Array& array_) 
        : array(array_) {}

    inline std::shared_ptr<IteratorBase> iter() override {
        return std::make_shared<array_iterator>(array);
    }
}; // END array_iterable

struct dictionary_iterator : public IteratorBase {
    Dictionary dict;
    Array keys;
    uint64_t index;
    Ref<ItEnd> itEnd;

    explicit dictionary_iterator(const Dictionary& dict_, uint64_t index_ = 0) 
        : dict(dict_), keys(dict.keys()), index(index_), itEnd(memnew(ItEnd)) {}
    
    inline Variant next() override {
        return index >= keys.size() ? Variant(this->itEnd) : Variant(Array::make(keys[index], dict[keys[index++]]));
    }
    inline bool has_next() override {
        return index < keys.size();
    }
}; // END dictionary_iterator

struct dictionary_iterable : public IterableBase {
    Dictionary dict;

    dictionary_iterable(const Dictionary& dict_) 
        : dict(dict_) {}

    inline std::shared_ptr<IteratorBase> iter() override {
        return std::make_shared<dictionary_iterator>(dict);
    }
}; // END dictionary_iterable

template<class WrapperT, class BaseT>
struct dictionary_mapping {
    Dictionary dict;

    dictionary_mapping(const Dictionary& dict_) 
        : dict(dict_) {}

    inline std::shared_ptr<BaseT> operator[](const Variant& key) const {
        if (auto wrapper = DYN_CAST(dict[key], WrapperT)) {
            return WrapperT::unwrap(wrapper);
        }
        throw BadArgumentException("Iterable contained element of wrong type!");
    }

    inline bool contains(const Variant& key) const {
        return dict.has(key);
    }

}; // END dictionary_mapping

template<class WrapperT, class BaseT>
struct array_container {
    Array arr;

    array_container(const Array& arr_) 
        : arr(arr_) {}
    array_container(const TypedArray<WrapperT>& arr_) 
        : arr(arr_) {}

    inline const std::shared_ptr<BaseT> operator[](int p_index) const {
        if (auto wrapper = DYN_CAST(arr[p_index], WrapperT)) {
            return WrapperT::unwrap(wrapper);
        }
        throw BadArgumentException("Iterable contained element of wrong type!");
    }

    inline size_t size() const {
        return arr.size();
    }

    inline bool empty() const {
        return arr.is_empty();
    }

}; // END array_container

/**
 * Utility type to wrap around IterableBase to enable C++-style iterations
 * for GDScript-style iterations. Only use this for wrapping between GodotAPI and Rx!
 * 
 * WARNING: Incremented iterator values work on the same IteratorBase instance!
*/
template<class WrapperT, class BaseT>
struct rx_wrapper_iterable {

    std::shared_ptr<IterableBase> it;

    rx_wrapper_iterable(const std::shared_ptr<IterableBase>& it_)
        : it(it_) {}
    
    struct const_iterator {
        std::shared_ptr<IteratorBase> it;
        bool is_end;
        Variant current;

        explicit const_iterator(const std::shared_ptr<IterableBase>& it_, bool is_end_ = false)
            :   it(is_end_ ? nullptr : it_->iter()), 
                is_end(is_end_ || !it->has_next()), 
                current(is_end ? Variant(Ref<ItEnd>(memnew(ItEnd))) : it->next()) {}
        
        bool operator!=(const const_iterator& other) const {
            return is_end ^ other.is_end;
        }
        const std::shared_ptr<BaseT> operator*() const {
            if (auto wrapper = DYN_CAST(current, WrapperT)) {
                return WrapperT::unwrap(wrapper);
            }
            throw BadArgumentException("Iterable contained element of wrong type!");
        }
        const_iterator& operator++() {
            if (is_end) {
                return *this;
            }
            is_end = !it->has_next();
            current = it->next();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator retval = *this; 
            ++(*this); 
            return retval;
        }
    };
    const_iterator begin() const {
        return const_iterator(this->it);
    }
    const_iterator end() const {
        return const_iterator(this->it, true);
    }

}; // END struct rx_wrapper_iterable

template<class T, Variant::Type type_id>
struct rx_iterable {

    std::shared_ptr<IterableBase> it;
    std::function<T(const Variant&)> mapper;

    rx_iterable(const std::shared_ptr<IterableBase>& it_, const std::function<T(const Variant&)>& mapper_)
        : it(it_), mapper(mapper_) {}
    
    struct const_iterator {
        std::function<T(const Variant&)> mapper;
        std::shared_ptr<IteratorBase> it;
        bool is_end;
        Variant current;

        explicit const_iterator(const std::function<T(const Variant&)>& mapper_, const std::shared_ptr<IterableBase>& it_, bool is_end_ = false)
            :   mapper(mapper_),
                it(is_end_ ? nullptr : it_->iter()), 
                is_end(is_end_ || !it->has_next()), 
                current(is_end ? Variant(Ref<ItEnd>(memnew(ItEnd))) : it->next()) {}
        
        bool operator!=(const const_iterator& other) const {
            return is_end ^ other.is_end;
        }
        const T operator*() const {
            if (current.get_type() == type_id) {
                return mapper(current);
            }
            throw BadArgumentException("Iterable contained element of wrong type!");
        }
        const_iterator& operator++() {
            if (is_end) {
                return *this;
            }
            is_end = !it->has_next();
            current = it->next();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator retval = *this; 
            ++(*this); 
            return retval;
        }
    };
    const_iterator begin() const {
        return const_iterator(mapper, this->it);
    }
    const_iterator end() const {
        return const_iterator(mapper, this->it, true);
    }
    
}; // END struct rx_iterable

namespace iterator {

static Ref<RxIterableBase> to_iterable(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterableBase)) {
        return iterable;
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIterableBase::wrap(std::make_shared<array_iterable>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIterableBase::wrap(std::make_shared<dictionary_iterable>(it));
    }
    return to_iterable(Array::make(it));
}

static Ref<RxIteratorBase> iter(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterableBase)) {
        return iterable->iter();
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIteratorBase::wrap(std::make_shared<array_iterator>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIteratorBase::wrap(std::make_shared<dictionary_iterator>(it));
    }
    return iter(Array::make(it));
}

} // END namespace iterator 

} // END namespace rx

#endif // RX_ITERATOR_H