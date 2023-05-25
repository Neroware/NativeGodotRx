#ifndef RX_ITERATOR_H
#define RX_ITERATOR_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/iterator.h"
#include "abstract/iterable.h"
#include "internal/basic.h"

using namespace godot;


class Iterator : public IteratorBase {
    GDCLASS(Iterator, IteratorBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("foreach", "cb"), &Iterator::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "cb"), &Iterator::enumerate);
    }

public:
    Iterator() {}
    ~Iterator() {}

    inline void foreach(const Callable& what) {
        while (this->has_next()) {
            Variant res = what.callv(Array::make(this->next()));
            if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
                break;
        }
    }
    inline void enumerate(const Callable& what) {
        uint64_t i = 0;
        while (this->has_next()) {
            Variant res = what.callv(Array::make(i++, this->next()));
            if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
                break;
        }
    }
};  // END Iterator


class ItEnd : public RefCounted {
    GDCLASS(ItEnd, RefCounted)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("ItEnd", D_METHOD("Get"), &ItEnd::Get);
    }

public:
    ItEnd() {}
    ~ItEnd() {}

    static inline Ref<ItEnd> Get() {
        return memnew(ItEnd);
    }
    inline String _to_string() const {
        return "<<ItEnd>>";
    }

}; // END ItEnd

class ArrayIterator : public Iterator {
    GDCLASS(ArrayIterator, Iterator)

private:
    uint64_t _count;
    const Array _array;
    const Variant _end;

protected:
	static void _bind_methods();

public:
    ArrayIterator(const Array& array) : _count(0), _array(array), _end(ItEnd::Get()) {}
    ArrayIterator() : _count(0), _end(ItEnd::Get()) {}
    ~ArrayIterator() {}

    static Ref<ArrayIterator> Get(const Array& array);

    Variant next() override;
    bool has_next() override;
    Variant front() override;
    Variant back() override;
    Variant end() override;
    bool empty() override;
}; // END ArrayIterator


class DictionaryIterator : public Iterator {
    GDCLASS(DictionaryIterator, Iterator)

private:
    uint64_t _count;
    const Dictionary _dict;
    const Array _keys;
    const Variant _end;

protected:
	static void _bind_methods();

public:
    DictionaryIterator(const Dictionary& dict) : _count(0), _dict(dict), _keys(dict.keys()), _end(ItEnd::Get()) {}
    DictionaryIterator() : _count(0), _end(ItEnd::Get()) {}
    ~DictionaryIterator() {}

    static Ref<DictionaryIterator> Get(const Dictionary& dict);

    Variant next() override;
    bool has_next() override;
    Variant front() override;
    Variant back() override;
    Variant end() override;
    bool empty() override;

}; // END DictionaryIterator

static inline Ref<Iterator> create_iterator(const Variant& iterable) {
    switch(iterable.get_type()) {
        case Variant::Type::ARRAY:
            return ArrayIterator::Get(iterable);
        case Variant::Type::DICTIONARY:
            return DictionaryIterator::Get(iterable);
    }
    if (auto it = CAST_OR_NULL(iterable, IterableBase)) {
        return it->iter();
    }
    return ArrayIterator::Get(Array::make(iterable));
}

#endif // RX_ITERATOR_H