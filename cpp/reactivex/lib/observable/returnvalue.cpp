#include "observable/definitions.h"

#include "scheduler/currentthreadscheduler.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::return_value_(const Variant& value, const scheduler_t& scheduler) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        scheduled_action_t action = ACTION(scheduler__, state) {
            observer->on_next(value);
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable