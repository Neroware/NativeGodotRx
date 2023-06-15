#ifndef RX_SCHEDULER_NEWTHREADSCHEDULER_H
#define RX_SCHEDULER_NEWTHREADSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "abstract/disposable.h"

#include "scheduler/periodicscheduler.h"

#include "internal/lambda.h"
#include "internal/concurrency.h"

class NewThreadScheduler : public PeriodicScheduler {
    GDCLASS(NewThreadScheduler, PeriodicScheduler)

private:
    Callable _thread_factory;

protected:
    static void _bind_methods();

public:
    NewThreadScheduler(const Callable& thread_factory = Lambda(VARIANT_FUN1(default_thread_factory))) : _thread_factory(thread_factory) {}
    ~NewThreadScheduler() {}

    static Ref<NewThreadScheduler> Get(const Callable& thread_factory = Lambda(VARIANT_FUN1(default_thread_factory)));

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state = Variant()) override;

};

#endif // RX_SCHEDULER_NEWTHREADSCHEDULER_H