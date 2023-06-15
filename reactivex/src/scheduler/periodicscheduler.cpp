#include "periodicscheduler.h"

#include <godot_cpp/variant/callable.hpp>

#include "disposable/multiassignmentdisposable.h"
#include "disposable/disposable.h"

#include "internal/lambda.h"

/**
 * NOTE: There is redundancy to Scheduler due to the fact that in this case multi-inheritance is avoided.
 * GDScript does not support multi-inheritance, thus PeriodicSchedulerBase directly inherits SchedulerBase.
 */

void PeriodicScheduler::_bind_methods() {
    ClassDB::bind_static_method("PeriodicScheduler", D_METHOD("Get"), &PeriodicScheduler::Get);
    ClassDB::bind_static_method("PeriodicScheduler", D_METHOD("to_seconds", "value"), &PeriodicScheduler::to_seconds);
    ClassDB::bind_static_method("PeriodicScheduler", D_METHOD("to_datetime", "value"), &PeriodicScheduler::to_datetime);
    ClassDB::bind_static_method("PeriodicScheduler", D_METHOD("to_timedelta", "value"), &PeriodicScheduler::to_timedelta);
    ClassDB::bind_method(D_METHOD("now"), &PeriodicScheduler::now);
    ClassDB::bind_method(D_METHOD("invoke_action", "action", "state"), &PeriodicScheduler::invoke_action, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_periodic", "period", "action", "state"), &PeriodicScheduler::schedule_periodic, DEFVAL(Variant()));
}

Ref<PeriodicScheduler> PeriodicScheduler::Get() {
    return memnew(PeriodicScheduler);
}

double PeriodicScheduler::to_seconds(const Variant& value) {
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t->to_sec();
    }
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return dt->to_sec();
    }
    return value.operator double();
}

Ref<AbsoluteTime> PeriodicScheduler::to_datetime(const Variant& value) {
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t;
    }
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return memnew(AbsoluteTime(UTC_ZERO + dt->dt));
    }
    return AbsoluteTime::Get(value.operator double());
}

Ref<RelativeTime> PeriodicScheduler::to_timedelta(const Variant& value) {
    if (auto dt = CAST_OR_NULL(value, RelativeTime)) {
        return dt;
    }
    if (auto t = CAST_OR_NULL(value, AbsoluteTime)) {
        return t->time_since_epoch();
    }
    return RelativeTime::Get(value.operator double());
}

Ref<AbsoluteTime> PeriodicScheduler::now() {
    return memnew(AbsoluteTime(basic::now<time_point_t>()));
}

Ref<DisposableBase> PeriodicScheduler::invoke_action(const Callable& action, const Variant& state) {
    auto res = action.callv(Array::make(Ref<SchedulerBase>(this), state));
    if (auto ret = CAST_OR_NULL(res, DisposableBase)) {
        return ret;
    }
    return Disposable::Get();
}

Ref<DisposableBase> PeriodicScheduler::schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state) {
    auto disp = MultiAssignmentDisposable::Get();
    auto seconds = to_seconds(period);

    VARIANT_FUN3 periodic = [=](Ref<SchedulerBase> scheduler, const Variant& state = Variant(), Callable periodic_cb = Callable()) {
        auto _disp = disp; auto _scheduler = scheduler;

        if (_disp->is_disposed) {
            return Variant();
        }

        auto _now = scheduler->now();

        Variant _state;
        try {
            _state = action.callv(Array::make(state));
        }
        catch(std::exception e) {
            _disp->dispose();
            throw e;
        }

        auto time = seconds - (**(_scheduler->now()) - **_now)->to_sec();
        _disp->set_disposable(
            _scheduler->schedule_relative(RelativeTime::Get(time), periodic_cb.bindv(Array::make(periodic_cb)), _state)
        );

        return Variant();
    };

    auto periodic_cb = Lambda(periodic);
    disp->set_disposable(
        this->schedule_relative(period, periodic_cb.bindv(Array::make(periodic_cb)), state)
    );
    return disp;
}