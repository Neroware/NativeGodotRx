#include "serialdisposable.h"

#include "disposable/autodisposer.h"

Ref<SerialDisposable> SerialDisposable::Get() {
    return memnew(SerialDisposable);
}

void SerialDisposable::_bind_methods() {
    ClassDB::bind_static_method("SerialDisposable", D_METHOD("Get"), &SerialDisposable::Get);
    ClassDB::bind_method(D_METHOD("get_disposable"), &SerialDisposable::get_disposable);
    ClassDB::bind_method(D_METHOD("set_disposable", "value"), &SerialDisposable::set_disposable);
    ClassDB::bind_method(D_METHOD("dispose"), &SerialDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &SerialDisposable::dispose_with);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "set_disposable", "get_disposable");

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &SerialDisposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &SerialDisposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &SerialDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");
}

Ref<DisposableBase> SerialDisposable::get_disposable() {
    return this->current;
}

void SerialDisposable::set_disposable(Ref<DisposableBase> value) {
    Ref<DisposableBase> old;

    bool should_dispose;
    {
        std::lock_guard<RLock> guard(**lock);
        should_dispose = this->is_disposed;
        if (!should_dispose) {
            old = this->current;
            this->current = value;
        }
    }

    if (!old.is_null()) {
        old->dispose();
    }

    if (should_dispose && !value.is_null()) {
        value->dispose();
    }
}

void SerialDisposable::dispose() {
    Ref<DisposableBase> old;

    {
        std::lock_guard<RLock> guard(**lock);
        if (!this->is_disposed) {
            this->is_disposed = true;
            old = this->current;
            this->current = Ref<DisposableBase>();
        }
    }

    if (!old.is_null()) {
        old->dispose();
    }
}

void SerialDisposable::dispose_with(Object* obj) {
    AutoDisposer::add_to(obj, this);
}

// Setters and Getters
bool SerialDisposable::__get__is_disposed__() {
    return this->is_disposed;
}
void SerialDisposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
Ref<RLock> SerialDisposable::__get__lock__() {
    return this->lock;
}