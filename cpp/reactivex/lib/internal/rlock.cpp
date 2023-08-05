#include "internal/rlock.h"

namespace rx {

void RLock::lock() {
    this->rm.lock();
}

void RLock::unlock() {
    this->rm.unlock();
}

bool RLock::try_lock() {
    return this->rm.try_lock();
}

} // END namespace rx