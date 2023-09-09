#include "disposable/refcountdisposable.h"

#include "disposable/disposable.h"

namespace rx::disposable {

void RefCountDisposable::InnerDisposable::dispose() {
    std::shared_ptr<RefCountDisposable> parent;
    {
        std::lock_guard<RLock> guard(lock);
        parent = this->parent;
        this->parent = nullptr;
    }

    if (parent) {
        parent->release();
    }
}

void RefCountDisposable::dispose() {
    if (this->is_disposed) {
        return;
    }

    std::shared_ptr<DisposableBase> _underlying_disposable;
    {
        std::lock_guard<RLock> guard(lock);
        if (!this->is_primary_disposed) {
            this->is_primary_disposed = true;
            if (!this->count) {
                this->is_disposed = true;
                _underlying_disposable = this->underlying_disposable;
            }
        }
    }

    if (_underlying_disposable) {
        _underlying_disposable->dispose();
    }
}

void RefCountDisposable::release() {
    if (this->is_disposed) {
        return;
    }

    bool should_dispose = false;
    {
        std::lock_guard<RLock> guard(lock);
        this->count--;
        if (!this->count && this->is_primary_disposed) {
            this->is_disposed = true;
            should_dispose = true;
        }
    }

    if (should_dispose) {
        this->underlying_disposable->dispose();
    }
}

disposable_t RefCountDisposable::get_disposable() {
    std::lock_guard<RLock> guard(lock);
    if (this->is_disposed) {
        return std::make_shared<Disposable>();
    }

    this->count++;
    return std::make_shared<InnerDisposable>(getptr());
}

} // END namespace rx::disposable