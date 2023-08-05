#include "scheduler/scenetreetimeoutscheduler.h"

#include "godotrx.h"

#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"

#include <chrono>
#include <thread>

namespace rx::scheduler {

std::shared_ptr<SceneTreeTimeoutScheduler> SceneTreeTimeoutScheduler::singleton(bool process_always, bool process_in_physics, bool ignore_time_scale) {
    uint8_t flags = 0b0;
    flags |= 0b100 * process_always;
    flags |= 0b10 * process_in_physics;
    flags |= 0b1 * ignore_time_scale;
    return GDRX->SceneTreeTimeoutScheduler_[flags];
}

std::shared_ptr<DisposableBase> SceneTreeTimeoutScheduler::schedule(const action_t& action, const Variant& state) {
    auto sad = std::make_shared<SingleAssignmentDisposable>();
    auto self = getptr();
    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(0.0, this->_process_always, this->_process_in_physics, this->_ignore_time_scale);

    Ref<RxSceneTreeTimeout> timeout(memnew(RxSceneTreeTimeout(timer, sad, self, action)));
    timer->connect("timeout", Callable(timeout.ptr(), "interval").bindv(Array::make(state)));
    
    dispose_t dispose = [timeout]() {
        timeout->cancel_timer();
    };

    return std::make_shared<CompositeDisposable>(disposable_list_t{sad, std::make_shared<Disposable>(dispose)});
}

std::shared_ptr<DisposableBase> SceneTreeTimeoutScheduler::schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state) {
    double seconds = to_seconds(duetime);
    if (seconds < 0.0) {
        return this->schedule(action, state);
    }

    auto sad = std::make_shared<SingleAssignmentDisposable>();
    auto self = getptr();
    SceneTree* sceneTree = static_cast<SceneTree*>(Engine::get_singleton()->get_main_loop());
    auto timer = sceneTree->create_timer(seconds, this->_process_always, this->_process_in_physics, this->_ignore_time_scale);

    Ref<RxSceneTreeTimeout> timeout(memnew(RxSceneTreeTimeout(timer, sad, self, action)));
    timer->connect("timeout", Callable(timeout.ptr(), "interval").bindv(Array::make(state)));
    
    dispose_t dispose = [timeout]() {
        timeout->cancel_timer();
    };

    return std::make_shared<CompositeDisposable>(disposable_list_t{sad, std::make_shared<Disposable>(dispose)});
}

std::shared_ptr<DisposableBase> SceneTreeTimeoutScheduler::schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state) {
    auto t = to_datetime(duetime);
    return this->schedule_relative(t - this->now(), action, state);
}

} // END namespace rx::scheduler