#ifndef RX_SCHEDULER_IMMEDIATESCHEDULER_H
#define RX_SCHEDULER_IMMEDIATESCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/scheduler.h"

class ImmediateScheduler : public Scheduler {
    GDCLASS(ImmediateScheduler, Scheduler)

protected:
    static void _bind_methods();

public:
    ImmediateScheduler(){}
    ~ImmediateScheduler(){}
    static Ref<ImmediateScheduler> Get();

    static Ref<ImmediateScheduler> singleton();

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
};

#endif // RX_SCHEDULER_IMMEDIATESCHEDULER_H