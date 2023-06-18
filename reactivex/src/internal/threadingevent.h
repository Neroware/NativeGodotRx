#ifndef RX_THREADINGEVENT_H
#define RX_THREADINGEVENT_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <condition_variable>
#include <mutex>

using namespace godot;

class ThreadingEvent : public RefCounted {
    GDCLASS(ThreadingEvent, RefCounted);

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    bool _flag;

protected:
    static void _bind_methods();

public:
    ThreadingEvent() : _flag(false) {}
    ~ThreadingEvent(){}
    static Ref<ThreadingEvent> Get();

    bool is_set();
    void set_flag();
    void clear();
    bool wait(double timeout = -1.0); 

};

#endif // RX_THREADINGEVENT_H