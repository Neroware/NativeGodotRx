#ifndef RX_SCHEDULER_CATCHSCHEDULER_H
#define RX_SCHEDULER_CATCHSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/periodicscheduler.h"
#include "internal/time.h"


using namespace godot;

class CatchScheduler : public PeriodicScheduler {
    GDCLASS(CatchScheduler, PeriodicScheduler)
    
private:
    Ref<SchedulerBase> _scheduler;
    Callable _handler;
    Ref<SchedulerBase> _recursive_original;
    Ref<CatchScheduler> _recursive_wrapper;

protected:
    static void _bind_methods();

public:

    CatchScheduler() {}
    CatchScheduler(Ref<SchedulerBase> scheduler, const Callable& handler) 
        : _scheduler(scheduler), _handler(handler) {}
    ~CatchScheduler() {}

    static Ref<CatchScheduler> Get(Ref<SchedulerBase> scheduler, const Callable& handler);

    Ref<AbsoluteTime> now() override;
    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state = Variant()) override;

private:
    Ref<CatchScheduler> _clone(Ref<SchedulerBase> scheduler) const;
    Callable _wrap(const Callable& action) const;
    Ref<CatchScheduler> _get_recursive_wrapper(Ref<SchedulerBase> scheduler);

};

#endif // RX_SCHEDULER_CATCHSCHEDULER_H