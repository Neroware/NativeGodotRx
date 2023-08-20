#ifndef RX_WRAPPER_SCHEDULER_H
#define RX_WRAPPER_SCHEDULER_H

#include "wrapper/wrapper.h"

#include "abstract/scheduler.h"
#include "exception/exception.h"
#include "wrapper/disposable.h"
#include "cast.h"

#include "scheduler/catchscheduler.h"
#include "scheduler/currentthreadscheduler.h"
#include "scheduler/immediatescheduler.h"
#include "scheduler/newthreadscheduler.h"
#include "scheduler/scenetreetimeoutscheduler.h"
#include "scheduler/timeoutscheduler.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::scheduler;

namespace rx {
namespace wrappers {

class RxSchedulerBase : public RefCounted {
    GDCLASS(RxSchedulerBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxSchedulerBase, SchedulerBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxSchedulerBase", D_METHOD("to_seconds", "value"), &RxSchedulerBase::to_seconds);
        ClassDB::bind_static_method("RxSchedulerBase", D_METHOD("to_datetime", "value"), &RxSchedulerBase::to_datetime);
        ClassDB::bind_static_method("RxSchedulerBase", D_METHOD("to_timedelta", "value"), &RxSchedulerBase::to_timedelta);
        ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &RxSchedulerBase::schedule, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &RxSchedulerBase::schedule_absolute, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &RxSchedulerBase::schedule_relative, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("schedule_periodic", "duetime", "action", "state"), &RxSchedulerBase::schedule_periodic, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("now"), &RxSchedulerBase::now);
        ClassDB::bind_method(D_METHOD("invoke_action", "action", "state"), &RxSchedulerBase::invoke_action, DEFVAL(Variant()));

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxSchedulerBase::equals);
    }
public:
    static double to_seconds(const Variant& value);
    static Ref<AbsoluteTime> to_datetime(const Variant& value);
    static Ref<RelativeTime> to_timedelta(const Variant& value);

    Ref<RxDisposableBase> schedule(const Callable& action, const Variant& state = Variant());
    Ref<RxDisposableBase> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant());
    Ref<RxDisposableBase> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant());
    Ref<RxDisposableBase> schedule_periodic(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant());
    Ref<AbsoluteTime> now();
    Ref<RxDisposableBase> invoke_action(const Callable& action, const Variant& state);

}; // END class RxSchedulerBase

class RxScheduler : public RxSchedulerBase {
    GDCLASS(RxScheduler, RxSchedulerBase)
    RX_WRAPPER(RxScheduler, Scheduler, RxSchedulerBase, SchedulerBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxScheduler", D_METHOD("ImmediateSchedulerSingleton"), &RxScheduler::ImmediateSchedulerSingleton);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("TimeoutSchedulerSingleton"), &RxScheduler::TimeoutSchedulerSingleton);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("SceneTimeoutSchedulerSingleton", "process_always", "process_in_physics", "ignore_time_scale"), &RxScheduler::SceneTimeoutSchedulerSingleton, DEFVAL(true), DEFVAL(false), DEFVAL(false));
        ClassDB::bind_static_method("RxScheduler", D_METHOD("CurrentThreadSchedulerSingleton"), &RxScheduler::CurrentThreadSchedulerSingleton);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("CurrentThreadScheduler"), &RxScheduler::CurrentThreadScheduler);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("NewThreadScheduler"), &RxScheduler::NewThreadScheduler);
    }

public:
    static Ref<RxScheduler> ImmediateSchedulerSingleton() { return RxScheduler::wrap(ImmediateScheduler::singleton()); }
    static Ref<RxScheduler> TimeoutSchedulerSingleton() { return RxScheduler::wrap(TimeoutScheduler::singleton()); }
    static Ref<RxScheduler> SceneTimeoutSchedulerSingleton(bool f0 = true, bool f1 = false, bool f2 = false) { return RxScheduler::wrap(SceneTreeTimeoutScheduler::singleton(f0, f1, f2)); }
    static Ref<RxScheduler> CurrentThreadSchedulerSingleton() { return RxScheduler::wrap(CurrentThreadScheduler::singleton()); }
    static Ref<RxScheduler> CurrentThreadScheduler() { return RxScheduler::wrap(CurrentThreadScheduler::get()); }
    static Ref<RxScheduler> NewThreadScheduler() { return RxScheduler::wrap(NewThreadScheduler::get()); }

}; // END class RxScheduler

} // END namespace wrappers

static action_t action_cb(const Callable& cb) {
    return action_t([cb](const std::shared_ptr<rx::abstract::SchedulerBase>& scheduler, const Variant& state = Variant()){
        auto res = cb.callv(Array::make(rx::wrappers::RxSchedulerBase::wrap(scheduler), state));
        if (auto casted = DYN_CAST_OR_NULL(res, rx::wrappers::RxDisposableBase)) {
            return casted->getptr();
        }
        return std::shared_ptr<rx::abstract::DisposableBase>();
    });
}

static periodic_action_t periodic_action_cb(const Callable& cb) {
    return periodic_action_t([cb](const Variant& state) -> Variant {
        return cb.callv(Array::make(state));
    });
}

} // END namespace rx
#endif // RX_WRAPPER_SCHEDULER_H