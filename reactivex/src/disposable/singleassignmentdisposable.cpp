#include "singleassignmentdisposable.h"

Ref<SingleAssignmentDisposable> SingleAssignmentDisposable::Get() {
    auto disp = memnew(SingleAssignmentDisposable);
    disp->is_disposed = false;
    disp->lock = RLock::Get();
    return disp;
}

void SingleAssignmentDisposable::_bind_methods() {
    ClassDB::bind_static_method("SingleAssignmentDisposable", D_METHOD("Get"), &SingleAssignmentDisposable::Get);
    ClassDB::bind_method(D_METHOD("get_disposable"), &SingleAssignmentDisposable::get_disposable);
    ClassDB::bind_method(D_METHOD("set_disposable", "value"), &SingleAssignmentDisposable::set_disposable);
    ClassDB::bind_method(D_METHOD("dispose"), &SingleAssignmentDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &SingleAssignmentDisposable::dispose_with);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "set_disposable", "get_disposable");

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &SingleAssignmentDisposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &SingleAssignmentDisposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &SingleAssignmentDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");
}

Ref<DisposableBase> SingleAssignmentDisposable::get_disposable() {
    return this->current;
}

void SingleAssignmentDisposable::set_disposable(Ref<DisposableBase> value) {
    if (!this->current.is_null()) {
        throw DisposedException();
    }

    this->lock->lock();
    bool should_dispose = this->is_disposed;
    if (!should_dispose) {
        this->current = value;
    }
    this->lock->unlock();

    if (this->is_disposed && !value.is_null()) {
        value->dispose();
    }
}

void SingleAssignmentDisposable::dispose() {
    Ref<DisposableBase> old;

    this->lock->lock();
    if (!this->is_disposed) {
        this->is_disposed = true;
        old = this->current;
        this->current = Ref<DisposableBase>();
    }
    this->lock->unlock();

    if (!old.is_null()) {
        old->dispose();
    }
}

void SingleAssignmentDisposable::dispose_with(Object* obj) {
    // TODO Implement AutoDisposer!!!
    throw NotImplementedException();
}

// Setters and Getters
bool SingleAssignmentDisposable::__get__is_disposed__() {
    return this->is_disposed;
}
void SingleAssignmentDisposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
Ref<RLock> SingleAssignmentDisposable::__get__lock__() {
    return this->lock;
}