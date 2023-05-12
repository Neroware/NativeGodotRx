#ifndef RX_SCHEDULER_SCHEDULEDITEM_H
#define RX_SCHEDULER_SCHEDULEDITEM_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "scheduler/scheduler.h"
#include "disposable/singleassignmentdisposable.h"


using namespace godot;

class ScheduledItem {

public:
    Ref<Scheduler> scheduler;
    Variant state;
    Callable action;
    Ref<AbsoluteTime> duetime;
    Ref<SingleAssignmentDisposable> disposable;

    ScheduledItem(Ref<Scheduler> scheduler, const Variant& state, const Callable& action, Ref<AbsoluteTime> duetime);

    void invoke();
    void cancel();
    bool is_cancelled();

    bool operator==(const ScheduledItem& other);
    bool operator<(const ScheduledItem& other);
    bool operator>(const ScheduledItem& other);
};

#endif // RX_SCHEDULER_SCHEDULEDITEM_H