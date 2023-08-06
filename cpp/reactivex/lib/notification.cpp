#include "notification.h"

#include "observer/observer.h"

namespace rx {

std::shared_ptr<ObservableBase> notification_t::to_observable(const std::shared_ptr<SchedulerBase>& scheduler) {
    // TODO
    throw NotImplementedException();
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
    return "OnError(" + String(this->exception.what()) + ")";
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

static std::shared_ptr<ObserverBase> from_notifier(const notification_handler_t& handler) {
    on_next_t _on_next = [=](const Variant& value) {
        handler(notification_on_next_t(value));
    };
    on_error_t _on_error = [=](const std::exception& e) {
        handler(notification_on_error_t(e));
    };
    on_completed_t _on_completed = [=]() {
        handler(notification_on_completed_t());
    };
    return rx::observer::Observer::get(_on_next, _on_error, _on_completed);
}

} // END namespace rx