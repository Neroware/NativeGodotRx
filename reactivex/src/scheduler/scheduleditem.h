#ifndef RX_SCHEDULER_SCHEDULEDITEM_H
#define RX_SCHEDULER_SCHEDULEDITEM_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "scheduler/scheduler.h"
#include "disposable/singleassignmentdisposable.h"


using namespace godot;

class ScheduledItem : public RefCounted{
    GDCLASS(ScheduledItem, RefCounted);

protected:
    static void _bind_methods();

public:
    Ref<Scheduler> scheduler;
    Variant state;
    Callable action;
    Ref<AbsoluteTime> duetime;
    Ref<SingleAssignmentDisposable> disposable;

    ScheduledItem() : disposable(SingleAssignmentDisposable::Get()) {}
    ScheduledItem(Ref<Scheduler> scheduler_, const Variant& state_, const Callable& action_, Ref<AbsoluteTime> duetime_) : scheduler(scheduler_), state(state_), action(action_), duetime(duetime_), disposable(SingleAssignmentDisposable::Get()) {}
    ~ScheduledItem(){}

    static Ref<ScheduledItem> Get(Ref<Scheduler> scheduler, const Variant& state, const Callable& action, Ref<AbsoluteTime> duetime);

    void invoke();
    void cancel();
    bool is_cancelled();

    Ref<Scheduler> __get__scheduler__();
    Variant __get__state__();
    Callable __get__action__();
    Ref<AbsoluteTime> __get__duetime__();
    Ref<SingleAssignmentDisposable> __get__disposable__();

    bool operator==(const ScheduledItem& other) const;
    bool operator<(const ScheduledItem& other) const;
    bool operator>(const ScheduledItem& other) const;

    struct compare {
        bool operator()(Ref<ScheduledItem> lhs, Ref<ScheduledItem> rhs) const {
            return **lhs < **rhs;
        }
    };
};

#endif // RX_SCHEDULER_SCHEDULEDITEM_H