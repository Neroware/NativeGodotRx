#ifndef RX_WRAPPER_SCHEDULER_H
#define RX_WRAPPER_SCHEDULER_H

#include "wrapper/abstract.h"

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
} // END namespace rx

#endif // RX_WRAPPER_SCHEDULER_H