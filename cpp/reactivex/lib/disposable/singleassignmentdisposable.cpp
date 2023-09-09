#include "disposable/singleassignmentdisposable.h"

namespace rx::disposable {

disposable_t SingleAssignmentDisposable::get_disposable() {
    return this->current;
}

void SingleAssignmentDisposable::set_disposable(const disposable_t& value) {
    if (this->current) {
        throw rx::exception::DisposedException();
    }

    bool should_dispose;
    {
        std::lock_guard<RLock> guard(lock);
        should_dispose = this->is_disposed;
        if (!should_dispose) {
            this->current = value;
        }
    }

    if (this->is_disposed && value) {
        value->dispose();
    }
}

void SingleAssignmentDisposable::dispose() {
    std::shared_ptr<DisposableBase> old;

    {
        std::lock_guard<RLock> guard(lock);
        if (!this->is_disposed) {
            this->is_disposed = true;
            old = this->current;
            this->current = nullptr;
        }
    }

    if (old) {
        old->dispose();
    }
}

} // END namespace rx::disposable