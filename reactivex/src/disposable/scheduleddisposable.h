#ifndef RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H
#define RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "internal/basic.h"
#include "internal/rlock.h"
#include "exception/exception.h"
#include "singleassignmentdisposable.h"

using namespace godot;

class ScheduledDisposable : public DisposableBase {
    GDCLASS(ScheduledDisposable, DisposableBase);

public:
    Ref<SchedulerBase> scheduler;
    Ref<SingleAssignmentDisposable> disposable;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    ScheduledDisposable(){}
    ~ScheduledDisposable(){}

    static Ref<ScheduledDisposable> Get(Ref<SchedulerBase> scheduler, Ref<DisposableBase> disposable);

    void dispose() override;
    void dispose_with(Object* obj) override;

    bool is_disposed();

    // Setters and Getters
    Ref<RLock> __get__lock__();
    Ref<SchedulerBase> __get__scheduler__();
    void __set__scheduler__(Ref<SchedulerBase> scheduler);
};

#endif // RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H