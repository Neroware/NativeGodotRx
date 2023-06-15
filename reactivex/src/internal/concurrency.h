#ifndef RX_CONCURRENCY_H
#define RX_CONCURRENCY_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/startable.h"
#include "internal/basic.h"
#include "internal/thread.h"
#include "internal/lambda.h"

using namespace godot;

class StartableThread : public StartableBase {
    GDCLASS(StartableThread, StartableBase);

private:
    Ref<RxThread> _thread;
    Callable _target;
    int _priority;

protected:
    inline static void _bind_methods() {
        ClassDB::bind_static_method("StartableThread", D_METHOD("Get", "target", "priority"), &StartableThread::Get, DEFVAL(Thread::PRIORITY_NORMAL));
        ClassDB::bind_method(D_METHOD("start"), &StartableThread::start);
        ClassDB::bind_method(D_METHOD("__get__thread__"), &StartableThread::__get__thread__);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "thread"), "", "__get__thread__");
    }

public:
    StartableThread() 
        : _thread(RxThread::Get()), _target(), _priority(Thread::PRIORITY_NORMAL) {}
    StartableThread(const Callable& target, int priority = Thread::PRIORITY_NORMAL)
        : _thread(RxThread::Get()), _target(target), _priority(priority) {}
    ~StartableThread() {}

    inline static Ref<StartableThread> Get(const Callable& target, int priority = Thread::PRIORITY_NORMAL) {
        return memnew(StartableThread(target, priority));
    }

    inline void start() override {
        this->_thread->start(this->_target, Thread::Priority(this->_priority));
    }

    inline Ref<RxThread> __get__thread__() {
        return this->_thread;
    }
};

inline static Ref<StartableBase> default_thread_factory(const Callable& target, int priority = Thread::PRIORITY_NORMAL) {
    return StartableThread::Get(target, priority);
}

#endif // RX_CONCURRENCY_H