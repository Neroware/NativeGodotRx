#include "observable/definitions.h"

#include "scheduler/immediatescheduler.h"

namespace rx::observable {

rx_observable_t obs::empty_(const scheduler_t& scheduler) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : rx::scheduler::ImmediateScheduler::singleton();

        scheduled_action_t action = [=](const scheduler_t& __, const Variant& ___) {
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

}