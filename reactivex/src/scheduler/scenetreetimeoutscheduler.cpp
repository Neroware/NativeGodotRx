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
    ClassDB::bind_static_method("SceneTreeTimeoutScheduler", D_METHOD("Get"), &SceneTreeTimeoutScheduler::Get);
    ClassDB::bind_static_method("SceneTreeTimeoutScheduler", D_METHOD("singleton"), &SceneTreeTimeoutScheduler::singleton);
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &SceneTreeTimeoutScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &SceneTreeTimeoutScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &SceneTreeTimeoutScheduler::schedule_absolute, DEFVAL(Variant()));
}

Ref<SceneTreeTimeoutScheduler> SceneTreeTimeoutScheduler::Get() {
    return memnew(SceneTreeTimeoutScheduler);
}

Ref<SceneTreeTimeoutScheduler> SceneTreeTimeoutScheduler::singleton() {
    return GDRX->SceneTreeTimeoutScheduler_;
}

Ref<DisposableBase> SceneTreeTimeoutScheduler::schedule(const Callable& action, const Variant& state) {
    auto sad = SingleAssignmentDisposable::Get();

    auto self = Ref<SceneTreeTimeoutScheduler>(this);
    auto interval = Lambda(VOID_FUN0([=]() {
        auto _sad = sad; auto _self = self;
        _sad->set_disposable(_self->invoke_action(action, state));
    }));

    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(0.0);
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
    auto interval = Lambda(VOID_FUN0([=]() {
        auto _sad = sad; auto _self = self;
        _sad->set_disposable(_self->invoke_action(action, state));
    }));

    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(seconds);
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