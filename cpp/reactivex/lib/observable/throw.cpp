#include "observable/definitions.h"

namespace rx::observable {

rx_observable_t obs::throw_(const error_t& error, const scheduler_t& scheduler) {

    subscription_t subscription = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : ImmediateScheduler::singleton();

        auto action = ACTION(scheduler__, state) {
            observer->on_error(error);
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscription);

}

} // END namespace rx::observable
