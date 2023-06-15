#include "newthreadscheduler.h"

#include "scheduler/eventloopscheduler.h"

void NewThreadScheduler::_bind_methods() {
    ClassDB::bind_static_method("NewThreadScheduler", D_METHOD("Get", "thread_factory"), &NewThreadScheduler::Get, DEFVAL(Lambda(VARIANT_FUN1(default_thread_factory))));
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &NewThreadScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &NewThreadScheduler::schedule_absolute, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &NewThreadScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_periodic", "period", "action", "state"), &NewThreadScheduler::schedule_periodic, DEFVAL(Variant()));
}

Ref<NewThreadScheduler> NewThreadScheduler::Get(const Callable& thread_factory) {
    return memnew(NewThreadScheduler(thread_factory));
}

Ref<DisposableBase> NewThreadScheduler::schedule(const Callable& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::Get(this->_thread_factory, true);
    return scheduler->schedule(action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::Get(this->_thread_factory, true);
    return scheduler->schedule_relative(duetime, action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto dt = to_datetime(duetime);
    return this->schedule_relative(**dt - **(this->now()), action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state) {
    // TODO
    return Ref<DisposableBase>();
}