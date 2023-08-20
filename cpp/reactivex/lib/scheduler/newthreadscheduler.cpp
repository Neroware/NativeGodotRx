#include "scheduler/newthreadscheduler.h"

#include "internal/threadingevent.h"
#include "scheduler/eventloopscheduler.h"

namespace rx::scheduler {

std::shared_ptr<DisposableBase> NewThreadScheduler::schedule(const action_t& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::get(this->_thread_factory, true);
    return scheduler->schedule(action, state);
}

std::shared_ptr<DisposableBase> NewThreadScheduler::schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::get(this->_thread_factory, true);
    return scheduler->schedule_relative(duetime, action, state);
}

std::shared_ptr<DisposableBase> NewThreadScheduler::schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state) {
    auto dt = to_datetime(duetime);
    return this->schedule_relative(dt - this->now(), action, state);
}

std::shared_ptr<DisposableBase> NewThreadScheduler::schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state_) {

    double seconds = to_seconds(period);
    auto disposed = std::make_shared<threading_event>();

    auto this_ptr = getptr();
    auto state = std::make_shared<Variant>(state_);
    auto timeout = std::make_shared<double>(seconds);

    run_t run = [=]() -> Variant {
        while(true) {
            if (*timeout > 0.0) {
                disposed->wait(*timeout);
            }
            if (disposed->is_set()) {
                return Variant();
            }

            auto time = this->now();
            
            *state = action(*state);

            *timeout = seconds - Scheduler::to_seconds(this->now() - time);
        }
        return Variant();
    };

    auto thread = this->_thread_factory(run);
    if (!thread) {
        throw BadArgumentException("Thread factory of EventLoopScheduler must return a StartableBase!");
    }
    thread->start();

    dispose_t dispose = [disposed]() {
        disposed->set_flag();
    };

    return std::make_shared<Disposable>(dispose);
}

} // END namespace rx::scheduler