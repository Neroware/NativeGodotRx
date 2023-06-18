#ifndef RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H
#define RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/periodicscheduler.h"

class SceneTreeTimeoutScheduler : public PeriodicScheduler {
    GDCLASS(SceneTreeTimeoutScheduler, PeriodicScheduler)

private:
    bool _process_always;
    bool _process_in_physics;
    bool _ignore_time_scale;

    inline void _cancel_timer(Ref<SceneTreeTimer> timer) const {
        auto connections = timer->get_signal_connection_list("timeout");
        for (auto i = 0ul; i < connections.size(); i++) {
            Dictionary conn = connections[i];
            timer->disconnect("timeout", conn["callable"]);
        }
    }

protected:
    static void _bind_methods();

public:
    SceneTreeTimeoutScheduler(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false) 
        : _process_always(process_always), _process_in_physics(process_in_physics), _ignore_time_scale(ignore_time_scale) {}
    ~SceneTreeTimeoutScheduler(){}
    static Ref<SceneTreeTimeoutScheduler> Get(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false);

    static Ref<SceneTreeTimeoutScheduler> singleton(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false);

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
};

#endif // RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H
