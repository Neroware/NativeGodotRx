#include "scenetreetimeoutscheduler.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

#include "godotrx.h"

#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"
#include "internal/lambda.h"

#include <chrono>
#include <thread>


void SceneTreeTimeoutScheduler::_bind_methods() {
    ClassDB::bind_static_method("SceneTreeTimeoutScheduler", D_METHOD("Get", "process_always", "process_in_physics", "ignore_time_scale"), &SceneTreeTimeoutScheduler::Get, DEFVAL(true), DEFVAL(false), DEFVAL(false));
    ClassDB::bind_static_method("SceneTreeTimeoutScheduler", D_METHOD("singleton", "process_always", "process_in_physics", "ignore_time_scale"), &SceneTreeTimeoutScheduler::singleton, DEFVAL(true), DEFVAL(false), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &SceneTreeTimeoutScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &SceneTreeTimeoutScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &SceneTreeTimeoutScheduler::schedule_absolute, DEFVAL(Variant()));
}

Ref<SceneTreeTimeoutScheduler> SceneTreeTimeoutScheduler::Get(bool process_always, bool process_in_physics, bool ignore_time_scale) {
    return memnew(SceneTreeTimeoutScheduler(process_always, process_in_physics, ignore_time_scale));
}

Ref<SceneTreeTimeoutScheduler> SceneTreeTimeoutScheduler::singleton(bool process_always, bool process_in_physics, bool ignore_time_scale) {
    uint8_t flags = 0b0;
    flags |= 0b100 * process_always;
    flags |= 0b10 * process_in_physics;
    flags |= 0b1 * ignore_time_scale;
    return GDRX->SceneTreeTimeoutScheduler_[flags];
}

Ref<DisposableBase> SceneTreeTimeoutScheduler::schedule(const Callable& action, const Variant& state) {
    auto sad = SingleAssignmentDisposable::Get();
    auto self = Ref<SceneTreeTimeoutScheduler>(this);
    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(0.0, this->_process_always, this->_process_in_physics, this->_ignore_time_scale);

    auto interval = Lambda(VOID_FUN0([=]() {
        auto _sad = sad; auto _self = self; auto _timer = timer;
        _sad->set_disposable(_self->invoke_action(action, state));
        auto connections = _timer->get_signal_connection_list("timeout");
        for (auto i = 0ul; i < connections.size(); i++) {
            Dictionary conn = connections[i];
            Callable cb = conn["callable"];
            _timer->disconnect("timeout", cb);
        }
    }));
    timer->connect("timeout", interval);
    
    auto dispose = Lambda(VOID_FUN0([=]() {
        auto _timer = timer;
        _timer->disconnect("timeout", interval);
    }));

    return CompositeDisposable::Get(Array::make(sad, Disposable::Get(dispose)));
}

Ref<DisposableBase> SceneTreeTimeoutScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    double seconds = to_seconds(duetime);
    if (seconds < 0.0) {
        return this->schedule(action, state);
    }

    auto sad = SingleAssignmentDisposable::Get();
    auto self = Ref<SceneTreeTimeoutScheduler>(this);
    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(seconds, this->_process_always, this->_process_in_physics, this->_ignore_time_scale);

    auto interval = Lambda(VOID_FUN0([=]() {
        auto _sad = sad; auto _self = self; auto _timer = timer;
        _sad->set_disposable(_self->invoke_action(action, state));
        auto connections = _timer->get_signal_connection_list("timeout");
        for (auto i = 0ul; i < connections.size(); i++) {
            Dictionary conn = connections[i];
            Callable cb = conn["callable"];
            _timer->disconnect("timeout", cb);
        }
    }));
    timer->connect("timeout", interval);
    
    auto dispose = Lambda(VOID_FUN0([=]() {
        auto _timer = timer;
        _timer->disconnect("timeout", interval);
    }));

    return CompositeDisposable::Get(Array::make(sad, Disposable::Get(dispose)));
}

Ref<DisposableBase> SceneTreeTimeoutScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto t = to_datetime(duetime);
    return this->schedule_relative(**t - **(this->now()), action, state);
}