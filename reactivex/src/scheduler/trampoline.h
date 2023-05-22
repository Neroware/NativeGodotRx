#ifndef RX_SCHEDULER_TRAMPOLINE_H
#define RX_SCHEDULER_TRAMPOLINE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <deque>
#include <queue>
#include <condition_variable>

#include "internal/time.h"
#include "internal/basic.h"
#include "scheduler/scheduleditem.h"
#include "internal/lock.h"

using namespace godot;

class Trampoline : public RefCounted {
    GDCLASS(Trampoline, RefCounted);

private:
    bool _idle;
    std::priority_queue<Ref<ScheduledItem>, std::vector<Ref<ScheduledItem>>, ScheduledItem::compare> _queue;
    Ref<Lock> _lock;
    std::condition_variable_any _condition;

protected:
	static void _bind_methods();

public:
    Trampoline() : _idle(true), _lock(Lock::Get()) {}
    ~Trampoline(){}

    static Ref<Trampoline> Get();

    bool idle();
    void run(Ref<ScheduledItem> item);

private:
    void _run();

};

#endif // RX_SCHEDULER_TRAMPOLINE_H