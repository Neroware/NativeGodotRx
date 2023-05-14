#include "scheduleditem.h"

Ref<ScheduledItem> ScheduledItem::Get(Ref<Scheduler> scheduler, const Variant& state, const Callable& action, Ref<AbsoluteTime> duetime) {
    auto item = memnew(ScheduledItem);
    item->scheduler = scheduler;
    item->state = state;
    item->action = action;
    item->duetime = duetime;
    item->disposable = SingleAssignmentDisposable::Get();
    return item;
}

void ScheduledItem::_bind_methods() {
    ClassDB::bind_static_method("ScheduledItem", D_METHOD("Get", "scheduler", "state", "action", "duetime"), &ScheduledItem::Get);
    ClassDB::bind_method(D_METHOD("invoke"), &ScheduledItem::invoke);
    ClassDB::bind_method(D_METHOD("cancel"), &ScheduledItem::cancel);
    ClassDB::bind_method(D_METHOD("is_cancelled"), &ScheduledItem::is_cancelled);

    ClassDB::bind_method(D_METHOD("__get__scheduler__"), &ScheduledItem::__get__scheduler__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scheduler"), "", "__get__scheduler__");
    ClassDB::bind_method(D_METHOD("__get__state__"), &ScheduledItem::__get__state__);
    ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "state"), "", "__get__state__");
    ClassDB::bind_method(D_METHOD("__get__action__"), &ScheduledItem::__get__action__);
    ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "action"), "", "__get__action__");
    ClassDB::bind_method(D_METHOD("__get__duetime__"), &ScheduledItem::__get__duetime__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "duetime"), "", "__get__duetime__");
    ClassDB::bind_method(D_METHOD("__get__disposable__"), &ScheduledItem::__get__disposable__);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "", "__get__disposable__");
}

void ScheduledItem::invoke() {
    auto ret = this->scheduler->invoke_action(this->action, this->state);
    this->disposable->set_disposable(ret);
}

void ScheduledItem::cancel() {
    this->disposable->dispose();
}

bool ScheduledItem::is_cancelled() {
    return this->disposable->is_disposed;
}

bool ScheduledItem::operator==(const ScheduledItem& other) const {
    return *(this->duetime) == *(other.duetime);
}
bool ScheduledItem::operator<(const ScheduledItem& other) const {
    return *(this->duetime) < *(other.duetime);
}
bool ScheduledItem::operator>(const ScheduledItem& other) const {
    return *(this->duetime) > *(other.duetime);
}

Ref<Scheduler> ScheduledItem::__get__scheduler__() {
    return this->scheduler;
}
Variant ScheduledItem::__get__state__() {
    return this->state;
}
Callable ScheduledItem::__get__action__() {
    return this->action;
}
Ref<AbsoluteTime> ScheduledItem::__get__duetime__() {
    return this->duetime;
}
Ref<SingleAssignmentDisposable> ScheduledItem::__get__disposable__() {
    return this->disposable;
}