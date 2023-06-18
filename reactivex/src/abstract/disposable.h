#ifndef RX_ABSTRACT_DISPOSABLE_H
#define RX_ABSTRACT_DISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "exception/exception.h"

using namespace godot;

class DisposableBase : public RefCounted {
    GDCLASS(DisposableBase, RefCounted);

public:
    DisposableBase(){}
    ~DisposableBase(){}

protected:
	static void _bind_methods() {
        BIND_VIRTUAL_METHOD(DisposableBase, dispose);
        BIND_VIRTUAL_METHOD(DisposableBase, dispose_with);
    }

public:
    virtual void dispose(){
        throw NotImplementedException();
    }
    virtual void dispose_with(Object* obj) {
        throw NotImplementedException();
    }
    inline void _notification(int p_what) {
        if (p_what == NOTIFICATION_PREDELETE) {
            this->dispose();
        }
    }
};

#endif // RX_ABSTRACT_DISPOSABLE_H