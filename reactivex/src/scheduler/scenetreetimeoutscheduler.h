#ifndef RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H
#define RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/scheduler.h"

class SceneTreeTimeoutScheduler : public Scheduler {
    GDCLASS(SceneTreeTimeoutScheduler, Scheduler)

protected:
    static void _bind_methods();

public:
    SceneTreeTimeoutScheduler(){}
    ~SceneTreeTimeoutScheduler(){}
    static Ref<SceneTreeTimeoutScheduler> Get();

    static Ref<SceneTreeTimeoutScheduler> singleton();

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
};

#endif // RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H
