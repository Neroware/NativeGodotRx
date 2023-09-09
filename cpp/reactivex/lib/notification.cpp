#include "notification.h"

#include "wrapper/notificationwrapper.h"

/* #include "observer/observer.h"
#include "scheduler/immediatescheduler.h"
#include "observable/observable.h" */

/* using namespace rx::scheduler;
using namespace rx::observable; */

namespace rx {

observable_t Notification::to_observable(const scheduler_t& scheduler) {
    
    /* auto _scheduler = scheduler ? scheduler : ImmediateScheduler::singleton();
    auto self = getptr();

    subscription_t  subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        action_t action = [=](const std::shared_ptr<rx::abstract::SchedulerBase>& __, const godot::Variant& ___) -> std::shared_ptr<DisposableBase> {
            self->accept(observer);
            if (self->kind == "N") {
                observer->on_completed();
            }
            return nullptr;
        };

        auto __scheduler = scheduler_ ? scheduler_ : _scheduler;
        return __scheduler->schedule(action);
    };

    return Observable::get(subscribe); */
    return nullptr; // TODO
}

void NotificationOnNext::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_next(this->value);
}
void NotificationOnNext::accept(const observer_t& observer) const {
    observer->on_next(this->value);
}
String NotificationOnNext::to_string() const {
    return "OnNext(" + UtilityFunctions::str(this->value) + ")";
}

void NotificationOnError::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_error(this->exception);
}
void NotificationOnError::accept(const observer_t& observer) const {
    observer->on_error(this->exception);
}
String NotificationOnError::to_string() const {
    String what_ = "??";
    try { std::rethrow_exception(this->exception); } 
    catch (const rx_exception& e) { what_ = e.type(); what_ += ":"; what_ += e.what(); }
    catch (const std::exception& e) { what_ = e.what(); }
    return "OnError(??:" + what_ + ")";
}

void NotificationOnCompleted::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_completed();
}
void NotificationOnCompleted::accept(const observer_t& observer) const {
    observer->on_completed();
}
String NotificationOnCompleted::to_string() const {
    return "OnCompleted()";
}

observer_t to_notifier(const notification_handler_t& handler) {
    on_next_t _on_next = [=](const Variant& value) {
        handler(NotificationOnNext::get(value));
    };
    on_error_t _on_error = [=](const std::exception_ptr& e) {
        handler(NotificationOnError::get(e));
    };
    on_completed_t _on_completed = [=]() {
        handler(NotificationOnCompleted::get());
    };
    return nullptr; // TODO
    // return rx::observer::Observer::get(_on_next, _on_error, _on_completed);
}

} // END namespace rx