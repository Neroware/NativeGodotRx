#ifndef RX_SCHEDULER_PERIODICSCHEDULER_H
#define RX_SCHEDULER_PERIODICSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "abstract/periodicscheduler.h"
#include "internal/time.h"

class PeriodicScheduler : public PeriodicSchedulerBase {
    GDCLASS(PeriodicScheduler, PeriodicSchedulerBase)

protected:
    static void _bind_methods();

public:
    PeriodicScheduler(){}
    ~PeriodicScheduler(){}
    static Ref<PeriodicScheduler> Get();

    Ref<AbsoluteTime> now() override;
    Ref<DisposableBase> invoke_action(const Callable& action, const Variant& state = Variant());

    static double to_seconds(const Variant& t);
    static Ref<RelativeTime> to_timedelta(const Variant& t);
    static Ref<AbsoluteTime> to_datetime(const Variant& t);

    Ref<DisposableBase> schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state = Variant()) override;
};

#endif // RX_SCHEDULER_PERIODICSCHEDULER_H
