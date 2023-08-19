#include "wrapper/scheduler.h"

#include "cast.h"

namespace rx::wrappers {

double RxSchedulerBase::to_seconds(const Variant& value) {
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t->to_sec();
    }
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return dt->to_sec();
    }
    return value.operator double();
}

Ref<AbsoluteTime> RxSchedulerBase::to_datetime(const Variant& value) {
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t;
    }
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return memnew(AbsoluteTime(UTC_ZERO + dt->dt));
    }
    return AbsoluteTime::from_seconds(value.operator double());
}

Ref<RelativeTime> RxSchedulerBase::to_timedelta(const Variant& value) {
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return dt;
    }
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t->time_since_epoch();
    }
    return RelativeTime::from_seconds(value.operator double());
}

Ref<RxDisposableBase> RxSchedulerBase::schedule(const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule(action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule_absolute(duetime->t, action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule_relative(duetime->dt, action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_periodic(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto periodic_scheduler = std::dynamic_pointer_cast<PeriodicSchedulerBase>(this->_ptr);
    if (!periodic_scheduler) {
        throw NotImplementedException();
    }
    return RxDisposableBase::wrap(periodic_scheduler->schedule_periodic(duetime->dt, periodic_action_cb(action), state));
}
Ref<AbsoluteTime> RxSchedulerBase::now() {
    return memnew(AbsoluteTime(this->_ptr->now()));
}
Ref<RxDisposableBase> RxSchedulerBase::invoke_action(const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->invoke_action(action_cb(action), state));
}

} // END namespace rx::wrappers