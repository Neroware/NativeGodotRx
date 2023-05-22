#ifndef RX_SCHEDULER_CURRENTTHREADSCHEDULER_H
#define RX_SCHEDULER_CURRENTTHREADSCHEDULER_H

#include <algorithm>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "internal/weakkeydictionary.h"
#include "scheduler/trampolinescheduler.h"

using namespace godot;

class CurrentThreadScheduler : public TrampolineScheduler {
    GDCLASS(CurrentThreadScheduler, TrampolineScheduler);

private:
    Ref<WeakKeyDictionary> _tramps;

protected:
	static void _bind_methods();

public:
    CurrentThreadScheduler() : _tramps(WeakKeyDictionary::Get()) {}
    ~CurrentThreadScheduler(){}
    static Ref<CurrentThreadScheduler> Get();

    static Ref<CurrentThreadScheduler> singleton();
    Ref<Trampoline> get_trampoline();

}; // END CurrentThreadScheduler


class _CurrentThreadScheduler_Local : public RefCounted {
    GDCLASS(_CurrentThreadScheduler_Local, RefCounted);

private:
    Ref<WeakKeyDictionary> _tramp;

protected:
	static void _bind_methods();

public:
    _CurrentThreadScheduler_Local(){}
    ~_CurrentThreadScheduler_Local(){}
    static Ref<_CurrentThreadScheduler_Local> Get();

    Ref<Trampoline> get_trampoline();

}; // END _CurrentThreadScheduler_Local


class CurrentThreadSchedulerSingleton : public CurrentThreadScheduler {
    GDCLASS(CurrentThreadSchedulerSingleton, CurrentThreadScheduler);

protected:
    static void _bind_methods();

public:
    CurrentThreadSchedulerSingleton(){}
    ~CurrentThreadSchedulerSingleton(){}
    static Ref<CurrentThreadSchedulerSingleton> Get();

    Ref<Trampoline> get_trampoline();

}; // END CurrentThreadSchedulerSingleton

#endif // RX_SCHEDULER_CURRENTTHREADSCHEDULER_H