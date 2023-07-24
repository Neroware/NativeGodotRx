#include <exception/exceptionwrapper.h>

#include <basic.h>

using namespace godot;

namespace rx::exception {

void RxError::_bind_methods() {
    ClassDB::bind_method(D_METHOD("what"), &RxError::what);
	ClassDB::bind_method(D_METHOD("type"), &RxError::type);
}

Ref<RxError> RxError::wrap(const std::exception& err) {
    return memnew(RxError(err));
}

std::exception RxError::unwrap(Ref<RxError> err) {
    return err->err;
}

StringName RxError::type() {
    return "RxError";
}

StringName RxError::what() {
    return err.what();
}

void RxError::raise() {
    rx::basic::default_error(this->err);
}

}; // END namespace rx::exception