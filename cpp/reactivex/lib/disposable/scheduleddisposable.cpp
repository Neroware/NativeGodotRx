#include "disposable/scheduleddisposable.h"

#include "basic.h"

namespace rx::disposable {

bool ScheduledDisposable::is_disposed() {
    return this->disposable.is_disposed;
}

void ScheduledDisposable::dispose() {
    auto self = this->getptr();
    scheduled_action_t action = [self](const scheduler_t& scheduler, const Variant& state = Variant()) {
        self->disposable.dispose();
        return nullptr;
    };
    this->scheduler->schedule(action);
}

} // END namespace rx::disposable