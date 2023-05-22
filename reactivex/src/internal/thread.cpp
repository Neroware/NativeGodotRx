#include "thread.h"

#include <cassert>

#include "godotrx.h"

void RxThread::_register_thread(const std::thread::id& thread_id) const {
    std::unique_lock<std::shared_mutex> writeLock(GDRX->thread_registry.first);
    GDRX->thread_registry.second[thread_id] = this;
}
void RxThread::_unregister_thread(const std::thread::id& thread_id) const {
    std::unique_lock<std::shared_mutex> writeLock(GDRX->thread_registry.first);
    GDRX->thread_registry.second.erase(thread_id);
}
Variant RxThread::_thread_callback() const {
    auto thread_id = std::this_thread::get_id();
    this->_register_thread(thread_id);
    Variant res = this->_cb.callv(Array());
    this->_unregister_thread(thread_id);
    return res;
}

void RxThread::_bind_methods() {
    ClassDB::bind_static_method("RxThread", D_METHOD("Get"), &RxThread::Get);
    ClassDB::bind_static_method("RxThread", D_METHOD("get_current_thread"), &RxThread::get_current_thread);
    ClassDB::bind_method(D_METHOD("_thread_callback"), &RxThread::_thread_callback);
    ClassDB::bind_method(D_METHOD("start", "callable", "priority"), &RxThread::start);
}

Ref<RxThread> RxThread::Get() {
    return memnew(RxThread);
}

Ref<RxThread> RxThread::get_current_thread() {
    std::shared_lock<std::shared_mutex> readLock(GDRX->thread_registry.first);
    auto thread_id = std::this_thread::get_id();
    try {
        return GDRX->thread_registry.second.at(thread_id);
    }
    catch(std::out_of_range) {
        assert(false && "Querying thread was NOT registered!");
    }
}

Error RxThread::start(const Callable& callable, Thread::Priority priority) {
    this->_cb = callable;
    return Thread::start(Callable(this, "_thread_callback"), priority);
}

void RxMainThread::_bind_methods() {
    ClassDB::bind_method(D_METHOD("start", "callable", "priority"), &RxMainThread::start);
    ClassDB::bind_method(D_METHOD("get_id"), &RxMainThread::get_id);
    ClassDB::bind_method(D_METHOD("is_started"), &RxMainThread::is_started);
    ClassDB::bind_method(D_METHOD("is_alive"), &RxMainThread::is_alive);
    ClassDB::bind_method(D_METHOD("wait_to_finish"), &RxMainThread::wait_to_finish);
}