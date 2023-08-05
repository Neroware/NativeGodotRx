#include "disposable/disposable.h"

namespace rx::disposable {

void Disposable::dispose() {
    bool disposed = false;
    {
        std::lock_guard<RLock> guard(lock);
        if (!this->is_disposed) {
            disposed = true;
            this->is_disposed = true;
        }
    }

    if (disposed) {
        this->action();
    }
}

void Disposable::cancel() {
    std::lock_guard<RLock> guard(lock);
    this->is_disposed = true;
}

} // END namespace rx::disposable