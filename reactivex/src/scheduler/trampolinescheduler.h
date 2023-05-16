#ifndef RX_SCHEDULER_TRAMPOLINESCHEDULER_H
#define RX_SCHEDULER_TRAMPOLINESCHEDULER_H

#include <algorithm>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/scheduler.h"
#include "scheduler/trampoline.h"
#include "internal/time.h"

using namespace godot;

class TrampolineScheduler : public Scheduler {
    GDCLASS(TrampolineScheduler, Scheduler);

protected:
	static void _bind_methods();

private:
    Ref<Trampoline> _tramp;

public:
    TrampolineScheduler(){}
    ~TrampolineScheduler(){}

    static Ref<TrampolineScheduler> Get();

    Ref<Trampoline> get_trampoline();
    Ref<DisposableBase> schedule(const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    Ref<DisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant()) override;
    
    bool schedule_required();
    Ref<Trampoline> ensure_trampoline(const Callable& action);
};

#endif // RX_SCHEDULER_TRAMPOLINESCHEDULER_H