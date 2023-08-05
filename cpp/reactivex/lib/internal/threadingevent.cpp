#include "internal/threadingevent.h"

#include <chrono>

namespace rx {

bool threading_event::is_set() const {
    return this->_flag;
}

void threading_event::set_flag() {
    std::unique_lock<std::mutex> lock(this->_mutex);
    this->_flag = true;
    this->_condition.notify_all();
}

void threading_event::clear() {
    std::unique_lock<std::mutex> lock(this->_mutex);
    this->_flag = false;
}

bool threading_event::wait(double timeout) {
    std::unique_lock<std::mutex> lock(this->_mutex);
    bool signaled = this->_flag;
	if (!signaled && timeout >= 0.0) {
        signaled = this->_condition.wait_for(lock, std::chrono::duration<double>(timeout)) == std::cv_status::no_timeout;
    }
    else if (!signaled) {
        this->_condition.wait(lock);
        signaled = true;
    }
    return signaled;
}

} // END namespace rx