#include "exception/exceptionwrapper.h"

#include "basic.h"

using namespace godot;

namespace rx::exception {

void RxError::_bind_methods() {
    ClassDB::bind_method(D_METHOD("what"), &RxError::what);
	ClassDB::bind_method(D_METHOD("type"), &RxError::type);
}

Ref<RxError> RxError::wrap(const std::exception_ptr& err) {
    return err ? memnew(RxError(err)) : Ref<RxError>();
}

std::exception_ptr RxError::unwrap(Ref<RxError> err) {
    return err.is_null() ? nullptr : err->_err;
}

StringName RxError::type() const {
    return this->_type;
}

StringName RxError::what() const {
    return this->_what;
}

void RxError::raise() const {
    rx::basic::default_error(this->_err);
}

}; // END namespace rx::exception