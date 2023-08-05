#include "disposable/compositedisposable.h"

namespace rx::disposable {

void CompositeDisposable::add(const disposable_t& item) {
    bool should_dispose = false;
    {
        std::lock_guard<RLock> guard(lock);
        if (this->is_disposed) {
            should_dispose = true;
        }
        else {
            this->disposable.push_back(item);
        }
    }
    if (should_dispose) {
        item->dispose();
    }
}

bool CompositeDisposable::remove(const disposable_t& item) {
    if (this->is_disposed) {
        return false;
    }

    bool should_dispose = false;
    {
        std::lock_guard<RLock> guard(lock);
        if (std::find(this->disposable.begin(), this->disposable.end(), item) != this->disposable.end()) {
            this->disposable.remove(item);
            should_dispose = true;
        }
    }

    if (should_dispose) {
        item->dispose();
    }

    return should_dispose;
}

void CompositeDisposable::dispose() {
    if (this->is_disposed) {
        return;
    }

    disposable_list_t current_disposable;
    {
        std::lock_guard<RLock> guard(lock);
        this->is_disposed = true;
        current_disposable = this->disposable;
        this->disposable.clear();
    }

    for (auto disp : current_disposable) {
        disp->dispose();
    }
}

void CompositeDisposable::clear() {
    disposable_list_t current_disposable;
    {
        std::lock_guard<RLock> guard(lock);
        current_disposable = this->disposable;
        this->disposable.clear();
    }

    for (auto disp : current_disposable) {
        disp->dispose();
    }
}

bool CompositeDisposable::contains(const disposable_t& item) const {
    return std::find(this->disposable.begin(), this->disposable.end(), item) != this->disposable.end();
}

disposable_list_t CompositeDisposable::to_list() const {
    return this->disposable;
}

int CompositeDisposable::size() const {
    return this->disposable.size();
}

} // END namespace rx::disposable