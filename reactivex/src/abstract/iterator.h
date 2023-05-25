#ifndef RX_ABSTRACT_ITERATOR_H
#define RX_ABSTRACT_ITERATOR_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "exception/exception.h"

using namespace godot;

class IteratorBase : public RefCounted {
    GDCLASS(IteratorBase, RefCounted);

public:
    IteratorBase(){}
    ~IteratorBase(){}

protected:
	static void _bind_methods() {
        BIND_VIRTUAL_METHOD(IteratorBase, next);
        BIND_VIRTUAL_METHOD(IteratorBase, has_next);
        BIND_VIRTUAL_METHOD(IteratorBase, front);
        BIND_VIRTUAL_METHOD(IteratorBase, back);
        BIND_VIRTUAL_METHOD(IteratorBase, empty);
    }

public:
    virtual Variant next() {
        throw NotImplementedException();
    }
    virtual bool has_next() {
        throw NotImplementedException();
    }
    virtual Variant front() {
        throw NotImplementedException();
    }
    virtual Variant back() {
        throw NotImplementedException();
    }
    virtual Variant end() {
        throw NotImplementedException();
    }
    virtual bool empty() {
        throw NotImplementedException();
    }
};

#endif // RX_ABSTRACT_ITERATOR_H