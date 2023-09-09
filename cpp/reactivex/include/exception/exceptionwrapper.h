#ifndef RX_EXCEPTION_EXCEPTIONWRAPPER_H
#define RX_EXCEPTION_EXCEPTIONWRAPPER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "exception/exception.h"
#include "typing.h"

using namespace godot;

namespace rx::exception {

class RxError : public RefCounted {
    GDCLASS(RxError, RefCounted);

private:
    error_t _err;
    String _type = "??";
    String _what;

    RxError(const error_t& err) : _err(err) {
        try { std::rethrow_exception(err); }
        catch (const rx_exception& e) { this->_type = e.type(); this->_what = e.what(); }
        catch (const std::exception& e) { this->_what = e.what(); }
    }

protected:
	static void _bind_methods();

public:
    RxError(){ throw NotImplementedException(); }
    ~RxError(){}

    StringName what() const;
    StringName type() const;
    static Ref<RxError> wrap(const error_t& err);
    static error_t unwrap(Ref<RxError> err);
    void raise() const;

    inline String _to_string() const { return "[" + this->type() + ": " + this->what() + "]"; }
};

}; // END namespace rx::exception

#endif // RX_EXCEPTION_EXCEPTIONWRAPPER_H