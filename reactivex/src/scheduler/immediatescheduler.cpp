#include "immediatescheduler.h"

#include "exception/exception.h"
#include "godotrx.h"

void ImmediateScheduler::_bind_methods() {
    ClassDB::bind_static_method("ImmediateScheduler", D_METHOD("Get"), &ImmediateScheduler::Get);
    ClassDB::bind_static_method("ImmediateScheduler", D_METHOD("singleton"), &ImmediateScheduler::singleton);
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &ImmediateScheduler::schedule);
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &ImmediateScheduler::schedule_absolute);
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &ImmediateScheduler::schedule_relative);
}

Ref<ImmediateScheduler> ImmediateScheduler::Get() {
    return memnew(ImmediateScheduler);
}

Ref<ImmediateScheduler> ImmediateScheduler::singleton() {
    return GDRX->ImmediateScheduler_;
}

Ref<DisposableBase> ImmediateScheduler::schedule(const Callable& action, const Variant& state) {
    return this->invoke_action(action, state);
}

Ref<DisposableBase> ImmediateScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto _duetime = Scheduler::to_timedelta(duetime);
    if (_duetime->dt > DELTA_ZERO) {
        throw WouldBlockException();
    }
    return this->invoke_action(action, state);
}

Ref<DisposableBase> ImmediateScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto _duetime = Scheduler::to_datetime(duetime);
    return this->schedule_relative(**_duetime - **(this->now()), action, state);
}