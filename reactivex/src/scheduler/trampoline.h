#ifndef RX_SCHEDULER_TRAMPOLINE_H
#define RX_SCHEDULER_TRAMPOLINE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <queue>

#include "internal/time.h"
#include "scheduler/scheduleditem.h"

using namespace godot;

class Trampoline : public RefCounted {
    GDCLASS(Trampoline, RefCounted);

private:
    bool _idle;
    std::priority_queue<ScheduledItem> _queue;


protected:
	static void _bind_methods();

public:

    Trampoline();
    ~Trampoline();

    static Trampoline* Get();

};

#endif // RX_SCHEDULER_TRAMPOLINE_H