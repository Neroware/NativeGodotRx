#include "rlock.h"

Ref<RLock> RLock::Get() {
    return memnew(RLock);
}

void RLock::_bind_methods() {
    ClassDB::bind_static_method("RLock", D_METHOD("Get"), &RLock::Get);
    ClassDB::bind_method(D_METHOD("lock"), &RLock::lock);
    ClassDB::bind_method(D_METHOD("unlock"), &RLock::unlock);
    ClassDB::bind_method(D_METHOD("try_lock"), &RLock::try_lock);
}

void RLock::lock() {
    this->rm.lock();
}

void RLock::unlock() {
    this->rm.unlock();
}

bool RLock::try_lock() {
    return this->rm.try_lock();
}