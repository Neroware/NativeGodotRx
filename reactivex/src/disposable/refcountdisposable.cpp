#include "refcountdisposable.h"

#include "disposable/disposable.h"
#include "disposable/autodisposer.h"

void InnerDisposable::_bind_methods() {
    ClassDB::bind_static_method("InnerDisposable", D_METHOD("Get", "parent"), &InnerDisposable::Get);
    ClassDB::bind_method(D_METHOD("dispose"), &InnerDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &InnerDisposable::dispose_with);

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &InnerDisposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &InnerDisposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &InnerDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");

    ClassDB::bind_method(D_METHOD("__get__parent__"), &InnerDisposable::__get__parent__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "parent"), "", "__get__parent__");
}

Ref<InnerDisposable> InnerDisposable::Get(Ref<RefCountDisposable> parent) {
    return memnew(InnerDisposable(parent));
}

void InnerDisposable::dispose() {
    Ref<RefCountDisposable> parent;
    {
        std::lock_guard<RLock> guard(**lock);
        parent = this->parent;
        this->parent = Ref<RefCountDisposable>();
    }

    if (!parent.is_null()) {
        parent->release();
    }
}

void InnerDisposable::dispose_with(Object* obj) {
    AutoDisposer::add_to(obj, this);
}

void RefCountDisposable::_bind_methods() {
    ClassDB::bind_static_method("RefCountDisposable", D_METHOD("Get", "disposable"), &RefCountDisposable::Get);
    ClassDB::bind_method(D_METHOD("dispose"), &RefCountDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &RefCountDisposable::dispose_with);
    ClassDB::bind_method(D_METHOD("release"), &RefCountDisposable::release);
    ClassDB::bind_method(D_METHOD("get_disposable"), &RefCountDisposable::get_disposable);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "", "get_disposable");

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &RefCountDisposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &RefCountDisposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &RefCountDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");

    ClassDB::bind_method(D_METHOD("__get__underlying_disposable__"), &RefCountDisposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "underlying_disposable"), "", "__get__underlying_disposable__");

    ClassDB::bind_method(D_METHOD("__get__is_primary_disposed__"), &RefCountDisposable::__get__is_primary_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_primary_disposed"), "", "__get__is_primary_disposed__");

    ClassDB::bind_method(D_METHOD("__get__count__"), &RefCountDisposable::__get__count__);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "", "__get__count__");
}

Ref<RefCountDisposable> RefCountDisposable::Get(Ref<DisposableBase> disposable) {
    return memnew(RefCountDisposable(disposable));
}

void RefCountDisposable::dispose() {
    if (this->is_disposed) {
        return;
    }

    Ref<DisposableBase> _underlying_disposable;
    {
        std::lock_guard<RLock> guard(**lock);
        if (!this->is_primary_disposed) {
            this->is_primary_disposed = true;
            if (!this->count) {
                this->is_disposed = true;
                _underlying_disposable = this->underlying_disposable;
            }
        }
    }

    if (!_underlying_disposable.is_null()) {
        _underlying_disposable->dispose();
    }
}

void RefCountDisposable::release() {
    if (this->is_disposed) {
        return;
    }

    bool should_dispose = false;
    {
        std::lock_guard<RLock> guard(**lock);
        this->count--;
        if (!this->count && this->is_primary_disposed) {
            this->is_disposed = true;
            should_dispose = true;
        }
    }

    if (should_dispose) {
        this->underlying_disposable->dispose();
    }
}

Ref<DisposableBase> RefCountDisposable::get_disposable() {
    std::lock_guard<RLock> guard(**lock);
    if (this->is_disposed) {
        return Disposable::Get();
    }

    this->count++;
    return InnerDisposable::Get(this);
}

void RefCountDisposable::dispose_with(Object* obj) {
    AutoDisposer::add_to(obj, this);
}

// Setters and Getters
Ref<DisposableBase> RefCountDisposable::__get__underlying_disposable__() {
    return this->underlying_disposable;
}
bool RefCountDisposable::__get__is_primary_disposed__() {
    return this->is_primary_disposed;
}
void RefCountDisposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
bool RefCountDisposable::__get__is_disposed__() {
    return this->is_disposed;
}
Ref<RLock> RefCountDisposable::__get__lock__() {
    return this->lock;
}
int RefCountDisposable::__get__count__() {
    return this->count;
}
Ref<RefCountDisposable> InnerDisposable::__get__parent__() {
    return this->parent;
}
void InnerDisposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
bool InnerDisposable::__get__is_disposed__() {
    return this->is_disposed;
}
Ref<RLock> InnerDisposable::__get__lock__() {
    return this->lock;
}