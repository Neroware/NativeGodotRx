#ifndef RX_ABSTRACT_EXCEPTIONWRAPPER_H
#define RX_ABSTRACT_EXCEPTIONWRAPPER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "exception/exception.h"

using namespace godot;

namespace rx::exception {

class RxError : public RefCounted {
    GDCLASS(RxError, RefCounted);

private:
    const std::exception& err = std::exception();
    RxError(const std::exception& err_) : err(err_) {}

protected:
	static void _bind_methods();

public:
    RxError(){}
    ~RxError(){}

    StringName what() const;
    StringName type() const;
    static Ref<RxError> wrap(const std::exception& err);
    static std::exception unwrap(Ref<RxError> err);
    void raise() const;

    inline String _to_string() const { return "[" + this->type() + ":" + this->what() + "]"; }
};

}; // END namespace rx::exception

#endif // RX_ABSTRACT_EXCEPTIONWRAPPER_H