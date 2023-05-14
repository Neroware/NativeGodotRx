#include "lock.h"

Ref<Lock> Lock::Get() {
    auto lk = memnew(Lock);
    lk->flag = false;
    return lk;
}

void Lock::_bind_methods() {
    ClassDB::bind_static_method("Lock", D_METHOD("Get"), &Lock::Get);
    ClassDB::bind_method(D_METHOD("lock"), &Lock::lock);
    ClassDB::bind_method(D_METHOD("unlock"), &Lock::unlock);
    ClassDB::bind_method(D_METHOD("try_lock"), &Lock::try_lock);
}

void Lock::lock() {
    std::unique_lock<std::mutex> lock(this->m);
    while (this->flag) {
        this->cv.wait(lock);
    }
    this->flag = true;
}

bool Lock::try_lock() {
    std::unique_lock<std::mutex> lock(this->m);
    if (this->flag) {
        return false;
    }
    this->flag = true;
    return true;
}

void Lock::unlock() {
    std::unique_lock<std::mutex> lock(this->m);
    this->flag = false;
    lock.unlock();
    this->cv.notify_all();
}
