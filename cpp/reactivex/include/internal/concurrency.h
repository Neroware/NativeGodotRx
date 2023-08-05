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

using namespace godot;
using namespace rx::abstract;

namespace rx {

class StartableThread : public StartableBase {

private:
    Ref<RxThread> _thread;
    run_t _target;
    int _priority;

public:
    StartableThread(const run_t& target, int priority = Thread::PRIORITY_NORMAL)
        : _thread(memnew(RxThread)), _target(target), _priority(priority) {}
    ~StartableThread() {}

    inline void start() override {
        this->_thread->start(this->_target, Thread::Priority(this->_priority));
    }

    inline Variant await() override {
        return this->_thread->wait_to_finish();
    }
};

static std::shared_ptr<StartableBase> default_thread_factory(const run_t& target) {
    return std::make_shared<StartableThread>(target, Thread::PRIORITY_NORMAL);
}

static run_t run_cb(const Callable& cb) {
    return run_t([cb]() -> Variant { return cb.callv(Array()); });
}

} // END namespace rx

#endif // RX_CONCURRENCY_H