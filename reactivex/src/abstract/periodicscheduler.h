#ifndef RX_ABSTRACT_PERIODICSCHEDULER_H
#define RX_ABSTRACT_PERIODICSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/scheduler.h"
#include "internal/time.h"

using namespace godot;

class PeriodicSchedulerBase : public SchedulerBase {
    GDCLASS(PeriodicSchedulerBase, SchedulerBase);

public:
    PeriodicSchedulerBase(){}
    ~PeriodicSchedulerBase(){}

protected:
    static void _bind_methods() {
        BIND_VIRTUAL_METHOD(PeriodicSchedulerBase, schedule_periodic);
    }

public:
    virtual Ref<DisposableBase> schedule_periodic(Ref<RelativeTime> periodic, const Callable& action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }

};

#endif // RX_ABSTRACT_PERIODICSCHEDULER_H