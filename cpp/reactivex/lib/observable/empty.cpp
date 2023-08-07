#include "observable/observable.h"

#include "scheduler/immediatescheduler.h"

namespace rx::observable {

std::shared_ptr<Observable> Observable::empty(const std::shared_ptr<SchedulerBase>& scheduler) {

    subscription_t subscribe = [=](const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) -> std::shared_ptr<DisposableBase> {
        auto _scheduler = scheduler ? scheduler : rx::scheduler::ImmediateScheduler::singleton();

        action_t action = [=](const std::shared_ptr<SchedulerBase>& scheduler, const Variant& state) {
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule(action);
    };

    return Observable::get(subscribe);
}

}