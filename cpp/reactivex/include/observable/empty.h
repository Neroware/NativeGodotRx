#ifndef RX_OBSERVABLE_EMPTY_H
#define RX_OBSERVABLE_EMPTY_H

#include "observable/observable.h"

#include "scheduler/immediatescheduler.h"

namespace rx::observable {

static std::shared_ptr<Observable> empty_(const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : rx::scheduler::ImmediateScheduler::singleton();

        action_t action = [=](const std::shared_ptr<SchedulerBase>& __, const Variant& ___) {
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_EMPTY_H