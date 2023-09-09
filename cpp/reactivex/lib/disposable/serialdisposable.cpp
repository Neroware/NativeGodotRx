#include "disposable/serialdisposable.h"

namespace rx::disposable {

disposable_t SerialDisposable::get_disposable() {
    return this->current;
}

void SerialDisposable::set_disposable(const disposable_t& value) {
    std::shared_ptr<DisposableBase> old;

    bool should_dispose;
    {
        std::lock_guard<RLock> guard(lock);
        should_dispose = this->is_disposed;
        if (!should_dispose) {
            old = this->current;
            this->current = value;
        }
    }

    if (old) {
        old->dispose();
    }

    if (should_dispose && value) {
        value->dispose();
    }
}

void SerialDisposable::dispose() {
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