#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"

using namespace rx::scheduler;

namespace rx::observable {

std::shared_ptr<Observable> Observable::return_value(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler_) {
    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler_ ? scheduler_ : scheduler ? scheduler : CurrentThreadScheduler::singleton();

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