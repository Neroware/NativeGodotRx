#ifndef RX_THREAD_H
#define RX_THREAD_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/thread.hpp>

#include <thread>

#include "exception/exception.h"
#include "abstract/startable.h"

using namespace godot;
using namespace rx::exception;

namespace rx {

class RxThread : public Thread {
    GDCLASS(RxThread, Thread);

private:
    run_t _cb;

private:
    void _register_thread(const std::thread::id& thread_id) const;
    void _unregister_thread(const std::thread::id& thread_id) const;
    Variant _thread_callback() const;

protected:
    static void _bind_methods();

public:
    RxThread() {}
    ~RxThread() {}

    static Ref<RxThread> get_current_thread();
    Error start_(const Callable& callable, Thread::Priority priority = (Thread::Priority)1);
    Error start(const run_t& callable, Thread::Priority priority = (Thread::Priority)1);

}; // END RxThread


class RxMainThread : public RxThread {
    GDCLASS(RxMainThread, RxThread);

protected:
    static void _bind_methods();

public:
    RxMainThread() {}
    ~RxMainThread() {}

    inline Error start(const Callable& callable, Thread::Priority priority = (Thread::Priority)1) {
        throw NotImplementedException();
    }
    inline String get_id() const {
        throw NotImplementedException();
    }
    inline bool is_started() const {
        throw NotImplementedException();
    }
    inline bool is_alive() const {
        throw NotImplementedException();
    }
    inline Variant wait_to_finish() {
        throw NotImplementedException();
    }

};  // END RxMainThread

} // END namespace rx

#endif // RX_THREAD_H