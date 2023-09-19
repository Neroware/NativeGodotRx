#include "observable/definitions.h"

#include "disposable/autodisposer.h"

namespace rx::observable {

rx_observable_t godot::from_signal_(Object* owner, const StringName& signal_name, bool track_owner, const scheduler_t& scheduler) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : GodotSignalScheduler::singleton();
        auto _signal_scheduler = std::dynamic_pointer_cast<GodotSignalSchedulerBase>(_scheduler);
        if (!_signal_scheduler) {
            throw NotImplementedException("Expected implementation of GodotSignalSchedulerBase");
        }

        periodic_action_t action = [=](const Variant& value) -> Variant {
            observer->on_next(value);
            return VNULL;
        };

        if (track_owner) {
            dispose_t on_owner_death = [=]() {
                observer->on_completed();
            };
            disposable::RxAutoDisposer::add_to(owner, std::make_shared<Disposable>(on_owner_death));
        }

        return _signal_scheduler->schedule_signal(owner, signal_name, action);
    };

    return Observable::get(subscribe);

}

} // END namespace rx::observable