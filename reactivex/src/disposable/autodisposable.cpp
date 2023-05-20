#include "autodisposable.h"

void AutoDisposable::_bind_methods() {
    ClassDB::bind_static_method("AutoDisposable", D_METHOD("Get", "on_dispose"), &AutoDisposable::Get);
    ClassDB::bind_static_method("AutoDisposable", D_METHOD("Add", "obj", "disp"), &AutoDisposable::Add);
    ClassDB::bind_method(D_METHOD("dispose"), &AutoDisposable::dispose);
    ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &AutoDisposable::dispose_with);
}

Ref<AutoDisposable> AutoDisposable::Get(const Callable& on_dispose) {
    return memnew(AutoDisposable([on_dispose](){
        on_dispose.callv(Array());
    }));
}

void AutoDisposable::Add(Object* obj, Ref<DisposableBase> disp) {
    auto ad = memnew(AutoDisposable([disp](){
        Ref<DisposableBase> _disp = disp;
        _disp->dispose();
    }));
    ad->dispose_with(obj);
}

void AutoDisposable::dispose() {
    bool disposed = false;
    {
        std::lock_guard<RLock> guard(**_lock);
        if (!this->_is_disposed) {
            disposed = true;
            this->_is_disposed = true;
        }
    }

    if (disposed) {
        this->_on_dispose();
    }
}

void AutoDisposable::dispose_with(Object* obj) {
    uint64_t id = this->get_instance_id();
    String meta_entry = "autodisposer" + String::num_uint64(id);
    obj->set_meta(meta_entry, Ref<AutoDisposable>(this));
}

void AutoDisposable::cancel(Object* obj) {
    {
        std::lock_guard<RLock> guard(**_lock);
        this->_is_disposed = true;
    }
    uint64_t id = this->get_instance_id();
    String meta_entry = "autodisposer" + String::num_uint64(id);
    obj->remove_meta(meta_entry);
}

void AutoDisposable::_notification(int p_what) {
    if (p_what == Object::NOTIFICATION_PREDELETE) {
        this->dispose();
    }
}