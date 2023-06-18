#ifndef RX_ABSTRACT_STARTABLE_H
#define RX_ABSTRACT_STARTABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "exception/exception.h"

using namespace godot;

class StartableBase : public RefCounted {
    GDCLASS(StartableBase, RefCounted);

public:
    StartableBase(){}
    ~StartableBase(){}

protected:
	static void _bind_methods() {
        BIND_VIRTUAL_METHOD(StartableBase, start);
        BIND_VIRTUAL_METHOD(StartableBase, await);
    }

public:
    virtual void start() {
        throw NotImplementedException();
    }
    virtual Variant await() {
        throw NotImplementedException();
    }
};

#endif // RX_ABSTRACT_STARTABLE_H