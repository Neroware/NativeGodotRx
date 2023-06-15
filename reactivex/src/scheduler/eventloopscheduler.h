#ifndef RX_SCHEDULER_EVENTLOOPSCHEDULER_H
#define RX_SCHEDULER_EVENTLOOPSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "abstract/startable.h"
#include "internal/concurrency.h"
#include "internal/lock.h"
#include "disposable/disposable.h"
#include "scheduler/periodicscheduler.h"
#include "scheduler/scheduleditem.h"

#include <condition_variable>
#include <queue>

class EventLoopScheduler : public PeriodicScheduler {
    GDCLASS(EventLoopScheduler, PeriodicScheduler)

private:
    bool _is_disposed;
    Callable _thread_factory;
    Ref<StartableBase> _thread;
    Ref<Lock> _lock;
    std::condition_variable_any _condition;
    std::priority_queue<Ref<ScheduledItem>, std::vector<Ref<ScheduledItem>>, ScheduledItem::compare> _queue;
    Array _ready_list;

    bool _exit_if_empty;

protected:
    static void _bind_methods();

public:
    EventLoopScheduler(const Callable& thread_factory = Lambda(VARIANT_FUN1(default_thread_factory)), bool exit_if_empty = false) 
        : _is_disposed(false), _thread_factory(thread_factory), _lock(Lock::Get()), _exit_if_empty(exit_if_empty) {}
    ~EventLoopScheduler(){}
    static Ref<EventLoopScheduler> Get(const Callable& thread_factory = Lambda(VARIANT_FUN1(default_thread_factory)), bool exit_if_empty = false);

    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;

private:
    bool _has_thread();
    void _ensure_thread();

public:
    void run();
    void dispose();
    Ref<Disposable> as_disposable();

};

#endif // RX_SCHEDULER_EVENTLOOPSCHEDULER_H