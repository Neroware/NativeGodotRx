#ifndef RX_CONCURRENCY_H
#define RX_CONCURRENCY_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/startable.h"
#include "internal/thread.h"

#include <mutex>
#include <thread>
#include <shared_mutex>
#include <map>
#include <condition_variable>
#include <atomic>
#include <cassert>

using namespace godot;
using namespace rx::abstract;

namespace rx {

class ThreadManager {

public:
    /* Thread registry singleton */
    std::pair<std::shared_mutex, std::unordered_map<std::thread::id, Ref<RxThread>>> thread_registry;

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    std::thread _cleanup_thread;
    bool _shutdown = false;
    std::vector<std::shared_ptr<StartableBase>> _finished_threads;

public:
    ThreadManager() : _cleanup_thread(&ThreadManager::_cleanup, this) {}

    ~ThreadManager() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _shutdown = true;
        }

        _condition.notify_all();
        _cleanup_thread.join();

        if (!this->_finished_threads.empty()) {
            assert(false && "should not happen!");
        }
    }

    /**
     * This method adds the thread object to the cleanup thread
     * allowing it to be joined and disposed properly.
     * 
     * This is mainly done due to Godot's Thread.wait_to_finish() shinanigans.
    */
    void finish(const std::shared_ptr<StartableBase>& thread) {
        std::lock_guard<std::mutex> lock(_mutex);
        _finished_threads.push_back(thread);
        _condition.notify_all();
    }

private:
    void _cleanup() {
        while (true) {
            std::shared_ptr<StartableBase> thread = nullptr;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _condition.wait(lock, [this]() {
                    return _shutdown || !_finished_threads.empty();
                });

                if (_shutdown) {
                    break; 
                }

                thread = _finished_threads.back();
                _finished_threads.pop_back();
            }
            
            thread->await();
        }
    }


}; // END class ThreadManager

class StartableThread : public StartableBase {

private:
    Ref<RxThread> _thread;
    run_t _target;
    int _priority;
    std::atomic_flag _joined = ATOMIC_FLAG_INIT;

public:
    StartableThread(const run_t& target, int priority = Thread::PRIORITY_NORMAL)
        : _thread(memnew(RxThread)), _target(target), _priority(priority) {}
    ~StartableThread() {}

    inline void start() override {
        this->_thread->start(this->_target, Thread::Priority(this->_priority));
    }
    inline void await() override {
        if (!this->_joined.test_and_set()) {
            this->_thread->wait_to_finish();
        }
    }
    
}; // END class StartableThread

static std::shared_ptr<StartableBase> default_thread_factory(const run_t& target) {
    return std::make_shared<StartableThread>(target, Thread::PRIORITY_NORMAL);
}

static run_t run_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return run_t([cb]() -> Variant { return cb.callv(Array()); });
}

} // END namespace rx

#endif // RX_CONCURRENCY_H