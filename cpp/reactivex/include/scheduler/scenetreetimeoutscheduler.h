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
#include <godot_cpp/variant/callable.hpp>

#include "disposable/singleassignmentdisposable.h"
#include "scheduler/periodicscheduler.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::exception;
using namespace rx::disposable;

namespace rx::scheduler {

inline static void _cancel_timer(Ref<SceneTreeTimer> timer) {
    auto connections = timer->get_signal_connection_list("timeout");
    for (auto i = 0ul; i < connections.size(); i++) {
        Dictionary conn = connections[i];
        timer->disconnect("timeout", conn["callable"]);
    }
}

class SceneTreeTimeoutScheduler : public PeriodicScheduler {

private:
    bool _process_always;
    bool _process_in_physics;
    bool _ignore_time_scale;

protected:
    SceneTreeTimeoutScheduler(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false) 
        : _process_always(process_always), _process_in_physics(process_in_physics), _ignore_time_scale(ignore_time_scale) {}
public:
    ~SceneTreeTimeoutScheduler(){}
    inline std::shared_ptr<SceneTreeTimeoutScheduler> getptr() { return std::static_pointer_cast<SceneTreeTimeoutScheduler>(PeriodicScheduler::getptr()); }
    inline static std::shared_ptr<SceneTreeTimeoutScheduler> get(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false) 
    { 
        return std::shared_ptr<SceneTreeTimeoutScheduler>(new SceneTreeTimeoutScheduler(process_always, process_in_physics, ignore_time_scale)); 
    }

    static std::shared_ptr<SceneTreeTimeoutScheduler> singleton(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false);

    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
};

class RxSceneTreeTimeout : public RefCounted {
    GDCLASS(RxSceneTreeTimeout, RefCounted)

private:
    Ref<SceneTreeTimer> _timer;
    std::shared_ptr<SingleAssignmentDisposable> _sad;
    std::shared_ptr<SceneTreeTimeoutScheduler> _scheduler;
    action_t _action;
    Variant _state;

protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_interval", "self"), &RxSceneTreeTimeout::interval);
    }

public:
    RxSceneTreeTimeout() { throw NotImplementedException(); }
    RxSceneTreeTimeout(Ref<SceneTreeTimer> timer, const std::shared_ptr<SingleAssignmentDisposable>& forward, const std::shared_ptr<SceneTreeTimeoutScheduler>& scheduler, const action_t& action, const Variant& state) :
        _timer(timer), _sad(forward), _scheduler(scheduler), _action(action), _state(state) {}
    ~RxSceneTreeTimeout(){}

    inline void interval(Ref<RxSceneTreeTimeout> __ = Ref<RxSceneTreeTimeout>()) const {
        _cancel_timer(this->_timer);
        _sad->set_disposable(_scheduler->invoke_action(_action, _state));
    }

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_SCENETREETIMEOUTSCHEDULER_H
