#include "currentthreadscheduler.h"

#include "godotrx.h"

Ref<CurrentThreadScheduler> CurrentThreadScheduler::Get() {
    return memnew(CurrentThreadScheduler);
}

Ref<CurrentThreadScheduler> CurrentThreadScheduler::singleton() {
    auto thread = RxThread::get_current_thread();
    auto class_map_ = GDRX->CurrentThreadScheduler_global_->get_value(GDRX->CurrentThreadScheduler_cls_);
    Ref<WeakKeyDictionary> class_map;
    if (auto _class_map = CAST_OR_NULL(class_map_, WeakKeyDictionary)) {
        class_map = Ref<WeakKeyDictionary>(_class_map);
    }
    else {
        class_map = WeakKeyDictionary::Get();
        GDRX->CurrentThreadScheduler_global_->set_pair(GDRX->CurrentThreadScheduler_cls_, class_map);
    }

    Ref<CurrentThreadScheduler> self;
    if (!class_map->has_key(thread)) {
        self = CurrentThreadSchedulerSingleton::Get();
        class_map->set_pair(thread, self);
    }
    else {
        self = class_map->get_value(thread);
    }

    return self;
}

Ref<Trampoline> CurrentThreadScheduler::get_trampoline() {
    auto thread = RxThread::get_current_thread();
    if (auto tramp = CAST_OR_NULL(this->_tramps->get_value(thread), Trampoline)) {
        return tramp;
    }
    auto tramp = Trampoline::Get();
    this->_tramps->set_pair(thread, tramp);
    return tramp;
}

Ref<_CurrentThreadScheduler_Local> _CurrentThreadScheduler_Local::Get() {
    return memnew(_CurrentThreadScheduler_Local);
}

Ref<Trampoline> _CurrentThreadScheduler_Local::get_trampoline() {
    auto thread = RxThread::get_current_thread();
    if (!this->_tramp->has_key(thread)) {
        this->_tramp->set_pair(thread, Trampoline::Get());
    }
    return REF_CAST(this->_tramp->get_value(thread), Trampoline);
}

Ref<CurrentThreadSchedulerSingleton> CurrentThreadSchedulerSingleton::Get() {
    return memnew(CurrentThreadSchedulerSingleton);
}

Ref<Trampoline> CurrentThreadSchedulerSingleton::get_trampoline() {
    return GDRX->CurrentThreadScheduler_local_->get_trampoline();
}

/* Method Bindings */

void CurrentThreadScheduler::_bind_methods() {
    ClassDB::bind_static_method("CurrentThreadScheduler", D_METHOD("Get"), &CurrentThreadScheduler::Get);
    ClassDB::bind_static_method("CurrentThreadScheduler", D_METHOD("singleton"), &CurrentThreadScheduler::singleton);
    ClassDB::bind_method(D_METHOD("get_trampoline"), &CurrentThreadScheduler::get_trampoline);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trampoline"), "", "get_trampoline");
}

void _CurrentThreadScheduler_Local::_bind_methods() {
    ClassDB::bind_static_method("_CurrentThreadScheduler_Local", D_METHOD("Get"), &_CurrentThreadScheduler_Local::Get);
    ClassDB::bind_method(D_METHOD("get_trampoline"), &_CurrentThreadScheduler_Local::get_trampoline);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trampoline"), "", "get_trampoline");
}

void CurrentThreadSchedulerSingleton::_bind_methods() {
    ClassDB::bind_static_method("CurrentThreadSchedulerSingleton", D_METHOD("Get"), &CurrentThreadSchedulerSingleton::Get);
    ClassDB::bind_method(D_METHOD("get_trampoline"), &CurrentThreadSchedulerSingleton::get_trampoline);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trampoline"), "", "get_trampoline");
}