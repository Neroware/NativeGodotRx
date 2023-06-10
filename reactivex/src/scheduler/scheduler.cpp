#include "scheduler.h"

#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "internal/basic.h"
#include "disposable/disposable.h"
#include "exception/exception.h"

Ref<Scheduler> Scheduler::Get() {
    return memnew(Scheduler);
}

void Scheduler::_bind_methods() {
    ClassDB::bind_static_method("Scheduler", D_METHOD("Get"), &Scheduler::Get);
    ClassDB::bind_static_method("Scheduler", D_METHOD("to_seconds", "value"), &Scheduler::to_seconds);
    ClassDB::bind_static_method("Scheduler", D_METHOD("to_datetime", "value"), &Scheduler::to_datetime);
    ClassDB::bind_static_method("Scheduler", D_METHOD("to_timedelta", "value"), &Scheduler::to_timedelta);
    ClassDB::bind_method(D_METHOD("now"), &Scheduler::now);
    ClassDB::bind_method(D_METHOD("invoke_action", "action", "state"), &Scheduler::invoke_action, DEFVAL(Variant()));
}

double Scheduler::to_seconds(const Variant& value) {
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t->to_sec();
    }
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return dt->to_sec();
    }
    return value.operator double();
}

Ref<AbsoluteTime> Scheduler::to_datetime(const Variant& value) {
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t;
    }
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return memnew(AbsoluteTime(UTC_ZERO + dt->dt));
    }
    return AbsoluteTime::Get(value.operator double());
}

Ref<RelativeTime> Scheduler::to_timedelta(const Variant& value) {
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return dt;
    }
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t->time_since_epoch();
    }
    return RelativeTime::Get(value.operator double());
}

Ref<AbsoluteTime> Scheduler::now() {
    return memnew(AbsoluteTime(basic::now<time_point_t>()));
}

Ref<DisposableBase> Scheduler::invoke_action(const Callable& action, const Variant& state) {
    auto res = action.callv(Array::make(Ref<SchedulerBase>(this), state));
    if (auto ret = CAST_OR_NULL(res, DisposableBase)) {
        return ret;
    }
    return Disposable::Get();
 }