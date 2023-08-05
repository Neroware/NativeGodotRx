#include "internal/lock.h"

namespace rx {

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

} // END namespace rx