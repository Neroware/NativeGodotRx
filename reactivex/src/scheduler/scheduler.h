#ifndef RX_SCHEDULER_SCHEDULER_H
#define RX_SCHEDULER_SCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/scheduler.h"
#include "internal/time.h"

using namespace godot;

class Scheduler : public SchedulerBase {
    GDCLASS(Scheduler, SchedulerBase);

public:
    static const int UTC_ZERO = 0;
    static const int UTC_DELTA = 0;

protected:
	static void _bind_methods();

public:

    Scheduler();
    ~Scheduler();

    static Scheduler* Get();

    Ref<AbsoluteTime> now() override;
    Ref<DisposableBase> invoke_action(Callable action, Variant state = Variant());

    static int64_t to_seconds(const Variant& t);
    static Ref<RelativeTime> to_timedelta(const Variant& t);
    static Ref<AbsoluteTime> to_datetime(const Variant& t);

};

#endif // RX_SCHEDULER_SCHEDULER_H