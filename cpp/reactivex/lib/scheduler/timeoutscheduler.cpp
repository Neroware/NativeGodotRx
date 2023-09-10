#include "scheduler/timeoutscheduler.h"

#include "godotrx.h"

#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"

#include <chrono>
#include <memory>
#include <thread>

namespace rx::scheduler {

std::shared_ptr<TimeoutScheduler> TimeoutScheduler::singleton() {
    return GDRX->TimeoutScheduler_;
}

disposable_t TimeoutScheduler::schedule(const scheduled_action_t& action, const Variant& state) {
    auto sad = std::make_shared<SingleAssignmentDisposable>();

    auto cancel = std::make_shared<bool>(false);
    auto self = getptr();
    auto interval = [=]() {
        std::this_thread::sleep_for(std::chrono::seconds(0));
        if (!(*cancel)) sad->set_disposable(self->invoke_action(action, state));
    };

    std::thread timer(interval);
    timer.detach();

    dispose_t dispose = [cancel]() {
        *cancel = true;
    };

    return std::make_shared<CompositeDisposable>(sad, std::make_shared<Disposable>(dispose));
}

disposable_t TimeoutScheduler::schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state) {
    double seconds = to_seconds(duetime);
    if (seconds < 0.0) {
        return this->schedule(action, state);
    }

    auto sad = std::make_shared<SingleAssignmentDisposable>();

    auto cancel = std::make_shared<bool>(false);
    auto self = getptr();
    auto interval = [=]() {
        std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
        if (!(*cancel)) sad->set_disposable(self->invoke_action(action, state));
    };

    std::thread timer(interval);
    timer.detach();

    dispose_t dispose = [cancel]() {
        *cancel = true;
    };

    return std::make_shared<CompositeDisposable>(sad, std::make_shared<Disposable>(dispose));
}

disposable_t TimeoutScheduler::schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto t = to_datetime(duetime);
    return this->schedule_relative(t - this->now(), action, state);
}

} // END namespace rx::scheduler