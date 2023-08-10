#ifndef RX_OBSERVABLE_RETURNVALUE_H
#define RX_OBSERVABLE_RETURNVALUE_H

#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"

using namespace rx::scheduler;

namespace rx::observable {

static std::shared_ptr<Observable> return_value_(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        action_t action = [=](const std::shared_ptr<rx::abstract::SchedulerBase>& scheduler, const godot::Variant& state) {
            observer->on_next(value);
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_RETURNVALUE_H