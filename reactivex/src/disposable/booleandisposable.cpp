#include "booleandisposable.h"

#include "disposable/autodisposer.h"

Ref<BooleanDisposable> BooleanDisposable::Get() {
    return memnew(BooleanDisposable);
}

void BooleanDisposable::_bind_methods() {
    ClassDB::bind_static_method("BooleanDisposable", D_METHOD("Get"), &BooleanDisposable::Get);
    ClassDB::bind_method(D_METHOD("dispose"), &BooleanDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &BooleanDisposable::dispose_with);

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &BooleanDisposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &BooleanDisposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &BooleanDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");
}

void BooleanDisposable::dispose() {
    this->is_disposed = true;
}

void BooleanDisposable::dispose_with(Object* obj) {
    AutoDisposer::add_to(obj, this);
}

// Setters and Getters
bool BooleanDisposable::__get__is_disposed__() {
    return this->is_disposed;
}
void BooleanDisposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
Ref<RLock> BooleanDisposable::__get__lock__() {
    return this->lock;
}