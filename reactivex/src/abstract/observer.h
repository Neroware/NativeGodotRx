#ifndef RX_ABSTRACT_OBSERVER_H
#define RX_ABSTRACT_OBSERVER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

using namespace godot;

class ObserverBase : public RefCounted {
    GDCLASS(ObserverBase, RefCounted);

public:
    ObserverBase(){}
    ~ObserverBase(){}

protected:
	static void _bind_methods() {
        BIND_VIRTUAL_METHOD(ObserverBase, on_next);
        BIND_VIRTUAL_METHOD(ObserverBase, on_error);
        BIND_VIRTUAL_METHOD(ObserverBase, on_completed);
    }

public:
    virtual void on_next(const Variant& i) {
        throw NotImplementedException();
    }
    virtual void on_error(Ref<RxError> e) {
        throw NotImplementedException();
    }
    virtual void on_completed() {
        throw NotImplementedException();
    }

};

#endif // RX_ABSTRACT_OBSERVER_H