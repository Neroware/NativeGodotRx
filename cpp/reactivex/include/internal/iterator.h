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

    explicit array_iterator(const Array& array_, uint64_t index_ = 0) 
        : array(array_), index(index_) {}
    
    inline Variant end() override {
        return Ref<ItEnd>(memnew(ItEnd()));
    }
    inline Variant next() override {
        return index >= array.size() ? this->end() : array[index++];
    }
    inline bool has_next() override {
        return index < array.size();
    }
}; // END array_iterator

struct dictionary_iterator : public IteratorBase {
    Dictionary dict;
    Array keys;
    uint64_t index;

    explicit dictionary_iterator(const Dictionary& dict_, uint64_t index_ = 0) 
        : dict(dict_), keys(dict.keys()), index(index_) {}
    
    inline Variant end() override {
        return Ref<ItEnd>(memnew(ItEnd()));
    }
    inline Variant next() override {
        return index >= keys.size() ? this->end() : Variant(Array::make(keys[index], dict[keys[index++]]));
    }
    inline bool has_next() override {
        return index < keys.size();
    }
}; // END dictionary_iterator

namespace iterator {

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