#include "observer/observer.h"

namespace rx::observer {

void Observer::on_next(const Variant& item) {
    if (!this->is_stopped) {
        this->_on_next_core(item);
    }
}

void Observer::_on_next_core(const Variant& item) {
    this->_handler_on_next(item);
}

void Observer::on_error(const std::exception_ptr& error) {
    if (!this->is_stopped) {
        this->is_stopped = true;
        this->_on_error_core(error);
    }
}

void Observer::_on_error_core(const std::exception_ptr& error) {
    this->_handler_on_error(error);
}

void Observer::on_completed() {
    if (!this->is_stopped) {
        this->is_stopped = true;
        this->_on_completed_core();
    }
}

void Observer::_on_completed_core() {
    this->_handler_on_completed();
}

void Observer::dispose() {
    this->is_stopped = true;
}

bool Observer::fail(const std::exception_ptr& error) {
    if (!this->is_stopped) {
        this->is_stopped = true;
        this->_on_error_core(error);
        return true;
    }

    return false;
}

void Observer::throw_error(const std::exception_ptr& error) const {
    std::rethrow_exception(error);
}

notifier_t Observer::to_notifier() {
    std::shared_ptr<ObserverBase> self = getptr();
    notifier_t notifier_fun = [self](const notification_t& notifier) {
        notifier.accept(self);
    };
    return notifier_fun;
}

std::shared_ptr<ObserverBase> Observer::as_observer() {
    auto self = getptr();
    on_next_t on_next_ = [self](const Variant& item) { self->on_next(item); };
    on_error_t on_error_ = [self](const std::exception_ptr& e) { self->on_error(e); };
    on_completed_t on_completed_ = [self]() { self->on_completed(); };
    return Observer::get(on_next_, on_error_, on_completed_);
}

} // END namespace Observer