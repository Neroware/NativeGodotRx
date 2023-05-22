#include "disposable.h"
#include "internal/basic.h"

#include <godot_cpp/variant/callable.hpp>

Ref<Disposable> Disposable::Get(const Callable& action) {
    return memnew(Disposable(action));
}

Ref<Disposable> Disposable::Empty() {
    return memnew(Disposable);
}

void Disposable::_bind_methods() {
    ClassDB::bind_static_method("Disposable", D_METHOD("Get", "action"), &Disposable::Get);
    ClassDB::bind_static_method("Disposable", D_METHOD("Empty"), &Disposable::Empty);
    ClassDB::bind_method(D_METHOD("dispose"), &Disposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &Disposable::dispose_with);

    ClassDB::bind_method(D_METHOD("__get__is_disposed__"), &Disposable::__get__is_disposed__);
    ClassDB::bind_method(D_METHOD("__set__is_disposed__", "v"), &Disposable::__set__is_disposed__);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "__set__is_disposed__", "__get__is_disposed__");

    ClassDB::bind_method(D_METHOD("__get__lock__"), &Disposable::__get__lock__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock"), "", "__get__lock__");

    ClassDB::bind_method(D_METHOD("__get__action__"), &Disposable::__get__action__);
    ClassDB::bind_method(D_METHOD("__set__action__", "v"), &Disposable::__set__action__);
    ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "action"), "__set__action__", "__get__action__");
}

void Disposable::dispose() {
    bool disposed = false;
    {
        std::lock_guard<RLock> guard(**lock);
        if (!this->is_disposed) {
            disposed = true;
            this->is_disposed = true;
        }
    }

    if (disposed) {
        this->action.callv(Array());
    }
}

void Disposable::dispose_with(Object* obj) {
    // TODO Implement AutoDisposer!!!
    throw NotImplementedException();
}

// Setters and Getters
bool Disposable::__get__is_disposed__() {
    return this->is_disposed;
}
void Disposable::__set__is_disposed__(bool is_disposed) {
    this->is_disposed = is_disposed;
}
Ref<RLock> Disposable::__get__lock__() {
    return this->lock;
}
Callable Disposable::__get__action__() {
    return this->action;
}
void Disposable::__set__action__(const Callable& action) {
    this->action = action;
}