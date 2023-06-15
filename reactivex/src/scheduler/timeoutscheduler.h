#ifndef RX_SCHEDULER_TIMEOUTSCHEDULER_H
#define RX_SCHEDULER_TIMEOUTSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/periodicscheduler.h"

class TimeoutScheduler : public PeriodicScheduler {
    GDCLASS(TimeoutScheduler, PeriodicScheduler)

protected:
    static void _bind_methods();

public:
    TimeoutScheduler(){}
    ~TimeoutScheduler(){}
    static Ref<TimeoutScheduler> Get();

    static Ref<TimeoutScheduler> singleton();

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
};

#endif // RX_SCHEDULER_TIMEOUTSCHEDULER_H
