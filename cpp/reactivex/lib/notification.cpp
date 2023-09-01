#include "notification.h"

#include "observer/observer.h"
#include "scheduler/immediatescheduler.h"
#include "observable/observable.h"

using namespace rx::scheduler;
using namespace rx::observable;

namespace rx {

std::shared_ptr<ObservableBase> notification_t::to_observable(const std::shared_ptr<SchedulerBase>& scheduler) {
    
    auto _scheduler = scheduler ? scheduler : ImmediateScheduler::singleton();
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

    return Observable::get(subscribe);
}

void notification_on_next_t::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_next(this->value);
}
void notification_on_next_t::accept(const std::shared_ptr<ObserverBase>& observer) const {
    observer->on_next(this->value);
}
String notification_on_next_t::to_string() const {
    return "OnNext(" + UtilityFunctions::str(this->value) + ")";
}

void notification_on_error_t::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_error(this->exception);
}
void notification_on_error_t::accept(const std::shared_ptr<ObserverBase>& observer) const {
    observer->on_error(this->exception);
}
String notification_on_error_t::to_string() const {
    String what_ = "??";
    try { std::rethrow_exception(this->exception); } 
    catch (const rx_exception& e) { what_ = e.type(); what_ += ":"; what_ += e.what(); }
    catch (const std::exception& e) { what_ = e.what(); }
    return "OnError(??:" + what_ + ")";
}

void notification_on_completed_t::accept(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed) const {
    on_completed();
}
void notification_on_completed_t::accept(const std::shared_ptr<ObserverBase>& observer) const {
    observer->on_completed();
}
String notification_on_completed_t::to_string() const {
    return "OnCompleted()";
}

std::shared_ptr<ObserverBase> from_notifier(const notification_handler_t& handler) {
    on_next_t _on_next = [=](const Variant& value) {
        handler(notification_on_next_t::get(value));
    };
    on_error_t _on_error = [=](const std::exception_ptr& e) {
        handler(notification_on_error_t::get(e));
    };
    on_completed_t _on_completed = [=]() {
        handler(notification_on_completed_t::get());
    };
    return rx::observer::Observer::get(_on_next, _on_error, _on_completed);
}

notification_handler_t notification_handler_cb(const Callable& handler) {
    return [handler](const std::shared_ptr<notification_t>& n) {
        handler.callv(Array::make(RxNotification::wrap(n)));
    };
}

} // END namespace rx