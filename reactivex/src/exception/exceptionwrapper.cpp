#include "exceptionwrapper.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void RxError::_bind_methods() {
    ClassDB::bind_method(D_METHOD("what"), &RxError::what);
	ClassDB::bind_method(D_METHOD("type"), &RxError::type);
}

Ref<RxError> RxError::wrap(const std::exception& err) {
    return memnew(RxError(err));
}

StringName RxError::type() {
    return "RxError";
}

StringName RxError::what() {
    return err.what();
}

void RxError::raise() {
    throw this->err;
}