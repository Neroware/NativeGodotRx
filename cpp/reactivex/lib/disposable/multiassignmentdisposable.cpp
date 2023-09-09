#include "disposable/multiassignmentdisposable.h"

namespace rx::disposable {

disposable_t MultiAssignmentDisposable::get_disposable() {
    return this->current;
}

void MultiAssignmentDisposable::set_disposable(const disposable_t& value) {
    bool should_dispose;
    {
        std::lock_guard<RLock> guard(lock);
        should_dispose = this->is_disposed;
        if (!should_dispose) {
            this->current = value;
        }
    }

    if (should_dispose && value) {
        value->dispose();
    }
}

void MultiAssignmentDisposable::dispose() {
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