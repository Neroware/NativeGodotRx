#include "scheduler/catchscheduler.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/disposable.h"

#include "exception/exception.h"

using namespace rx::exception;
using namespace rx::disposable;

namespace rx::scheduler {

time_point_t CatchScheduler::now() {
    return this->_scheduler->now();
}

disposable_t CatchScheduler::schedule(const scheduled_action_t& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule(action_, state);
}

disposable_t CatchScheduler::schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule_absolute(duetime, action_, state);
}

disposable_t CatchScheduler::schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule_relative(duetime, action_, state);
}

disposable_t CatchScheduler::schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state) {
    if (!std::dynamic_pointer_cast<PeriodicScheduler>(this->_scheduler)) {
        throw NotImplementedException();
    }

    auto disp = std::make_shared<SingleAssignmentDisposable>();
    auto failed = std::make_shared<bool>(false);
    auto this_ptr = getptr();

    periodic_action_t periodic = [=](const Variant& state) -> Variant {
        if (*failed) {
            return nullptr;
        }
        try {
            return action(state);
        }
        catch(...) {
            *failed = true;
            if (!this_ptr->_handler(std::current_exception())) {
                throw;
            }
            disp->dispose();
            return nullptr;
        }
    };

    auto scheduler = std::static_pointer_cast<PeriodicScheduler>(this->_scheduler);
    disp->set_disposable(
        scheduler->schedule_periodic(period, periodic, state)
    );
    return disp;
}

std::shared_ptr<CatchScheduler> CatchScheduler::_clone(const scheduler_t& scheduler) const {
    return CatchScheduler::get(scheduler, this->_handler);
}

scheduled_action_t CatchScheduler::_wrap(const scheduled_action_t& action) {
    auto parent = getptr();

    scheduled_action_t wrapped_action = [=](const scheduler_t& self, const Variant& state = Variant()) -> disposable_t {
        try {
            return action(parent->_get_recursive_wrapper(self), state);
        }
        catch(...) {
            if (!parent->_handler(std::current_exception())) {
                throw;
            }
            return std::make_shared<Disposable>();
        }
    };

    return wrapped_action;
}

std::shared_ptr<CatchScheduler> CatchScheduler::_get_recursive_wrapper(const scheduler_t& scheduler) {
    if (!this->_recursive_wrapper || this->_recursive_original != scheduler) {
        this->_recursive_original = scheduler;
        auto wrapper = this->_clone(scheduler);
        wrapper->_recursive_original = scheduler;
        wrapper->_recursive_wrapper = wrapper;
        this->_recursive_wrapper = wrapper;
    }

    return this->_recursive_wrapper;
}

} // END namespace rx::scheduler