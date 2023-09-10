#include "observer/autodetachobserver.h"

namespace rx::observer {

void AutoDetachObserver::on_next(const Variant& value) {
    if (this->is_stopped)
        return;
    this->_on_next(value);
}

void AutoDetachObserver::on_error(const error_t& error) {
    if (this->is_stopped)
        return;
    this->is_stopped = true;

    this->_on_error(error);
}

void AutoDetachObserver::on_completed() {
    if (this->is_stopped)
        return;
    this->is_stopped = true;

    this->_on_completed();
}

void AutoDetachObserver::set_disposable(const disposable_t& disp) {
    this->_subscription.set_disposable(disp);
}

void AutoDetachObserver::dispose() {
    this->is_stopped = true;
    this->_subscription.dispose();
}

bool AutoDetachObserver::fail(const error_t& err) {
    if (this->is_stopped) 
        return false;
    
    this->is_stopped = true;
    this->_on_error(err);
    return true;
}

} // END namespace rx::observer