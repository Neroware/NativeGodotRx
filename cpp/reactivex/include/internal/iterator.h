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

/**
 * Utility type to wrap around IterableBase to enable C++-style iterations
 * for GDScript-style iterations. Only use this for wrapping between GodotAPI and Rx!
 * 
 * WARNING: Incremented iterator values work on the same IteratorBase instance!
*/
template<class WrapperT, class BaseT>
struct rx_iterable {

    std::shared_ptr<IterableBase> it;

    rx_iterable(const std::shared_ptr<IterableBase>& it_)
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



}; // END rx_iterable

namespace iterator {

static Ref<RxIterable> to_iterable(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterable)) {
        return iterable;
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIterable::wrap(std::make_shared<array_iterable>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIterable::wrap(std::make_shared<dictionary_iterable>(it));
    }
    return to_iterable(Array::make(it));
}

static Ref<RxIterator> iter(const Variant& it) {
    if (auto iterable = DYN_CAST_OR_NULL(it, RxIterable)) {
        return iterable->iter();
    }
    if (it.get_type() == Variant::ARRAY) {
        return RxIterator::wrap(std::make_shared<array_iterator>(it));
    }
    if (it.get_type() == Variant::DICTIONARY) {
        return RxIterator::wrap(std::make_shared<dictionary_iterator>(it));
    }
    return iter(Array::make(it));
}

} // END namespace iterator 

} // END namespace rx

#endif // RX_ITERATOR_H