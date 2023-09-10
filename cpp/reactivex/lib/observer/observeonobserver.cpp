#include "observer/observeonobserver.h"

namespace rx::observer {

void ObserveOnObserver::_on_next_core(const Variant& item) {
    this->ensure_active();
    ScheduledObserver::_on_next_core(item);
}

void ObserveOnObserver::_on_error_core(const error_t& error) {
    this->ensure_active();
    ScheduledObserver::_on_error_core(error);
}

void ObserveOnObserver::_on_completed_core() {
    this->ensure_active();
    ScheduledObserver::_on_completed_core();
}

} // END namespace rx::observer