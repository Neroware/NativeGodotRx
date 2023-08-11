#ifndef RX_OBSERVABLE_THROW_H
#define RX_OBSERVABLE_THROW_H

#include "observable/observable.h"

namespace rx::observable {

static std::shared_ptr<Observable> throw_(const std::exception_ptr& exception, const std::shared_ptr<SchedulerBase>& scheduler) {

    subscription_t subscription = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler ? scheduler : ImmediateScheduler::singleton();

        auto action = ACTION {
            observer->on_error(exception);
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscription);

}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_THROW_H