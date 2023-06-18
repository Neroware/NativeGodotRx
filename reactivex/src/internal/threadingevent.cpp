#include "threadingevent.h"

#include <chrono>

void ThreadingEvent::_bind_methods() {
    ClassDB::bind_static_method("ThreadingEvent", D_METHOD("Get"), &ThreadingEvent::Get);
    ClassDB::bind_method(D_METHOD("is_set"), &ThreadingEvent::is_set);
    ClassDB::bind_method(D_METHOD("set_flag"), &ThreadingEvent::set_flag);
    ClassDB::bind_method(D_METHOD("clear"), &ThreadingEvent::clear);
    ClassDB::bind_method(D_METHOD("wait", "timeout"), &ThreadingEvent::wait, DEFVAL(-1.0));
}

Ref<ThreadingEvent> ThreadingEvent::Get() {
    return memnew(ThreadingEvent);
}

bool ThreadingEvent::is_set() {
    return this->_flag;
}

void ThreadingEvent::set_flag() {
    std::unique_lock<std::mutex> lock(this->_mutex);
    this->_flag = true;
    this->_condition.notify_all();
}

void ThreadingEvent::clear() {
    std::unique_lock<std::mutex> lock(this->_mutex);
    this->_flag = false;
}

bool ThreadingEvent::wait(double timeout) {
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