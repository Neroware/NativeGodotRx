#ifndef RX_WRAPPER_SCHEDULER_H
#define RX_WRAPPER_SCHEDULER_H

#include "wrapper/abstract.h"

#include "scheduler/catchscheduler.h"
#include "scheduler/currentthreadscheduler.h"
#include "scheduler/immediatescheduler.h"
#include "scheduler/newthreadscheduler.h"
#include "scheduler/scenetreetimeoutscheduler.h"
#include "scheduler/timeoutscheduler.h"
#include "scheduler/eventloopscheduler.h"
#include "scheduler/godotsignalscheduler.h"

#include "templates/scheduler.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::scheduler;
using namespace rx::templates;

namespace rx {
namespace wrappers {

class RxScheduler : public RxSchedulerBase {
    GDCLASS(RxScheduler, RxSchedulerBase)
    RX_WRAPPER(RxScheduler, Scheduler, RxSchedulerBase, SchedulerBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxScheduler)
    }
public:

}; // END class RxScheduler

class RxCatchScheduler : public RxScheduler {
    GDCLASS(RxCatchScheduler, RxScheduler)
    RX_WRAPPER(RxCatchScheduler, CatchScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxCatchScheduler", D_METHOD("get", "scheduler", "handler"), &RxCatchScheduler::get);
    }

public:
    inline static Ref<RxCatchScheduler> get(Ref<RxSchedulerBase> scheduler, const Callable& handler) {
        return RxCatchScheduler::wrap(CatchScheduler::get(
            RxSchedulerBase::unwrap(scheduler),
            handler_cb(handler)
        ));
    }

}; // END class RxCatchScheduler

class RxTrampolineScheduler : public RxScheduler {
    GDCLASS(RxTrampolineScheduler, RxScheduler)
    RX_WRAPPER(RxTrampolineScheduler, TrampolineScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxTrampolineScheduler", D_METHOD("get"), &RxTrampolineScheduler::get);
        ClassDB::bind_method(D_METHOD("schedule_required"), &RxTrampolineScheduler::schedule_required);
        ClassDB::bind_method(D_METHOD("ensure_trampoline"), &RxTrampolineScheduler::ensure_trampoline);
        RX_WRAPPER_CAST_BINDS(RxTrampolineScheduler)
    }

public:
    inline static Ref<RxTrampolineScheduler> get() {
        return RxTrampolineScheduler::wrap(TrampolineScheduler::get());
    }
    inline bool schedule_required() { return this->_ptr->schedule_required(); }
    inline void ensure_trampoline(const Callable& action) { this->_ptr->ensure_trampoline(scheduled_action_cb(action)); }

}; // END class RxTrampolineScheduler


class RxCurrentThreadScheduler : public RxTrampolineScheduler {
    GDCLASS(RxCurrentThreadScheduler, RxTrampolineScheduler)
    RX_WRAPPER(RxCurrentThreadScheduler, CurrentThreadScheduler, RxTrampolineScheduler, TrampolineScheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxCurrentThreadScheduler", D_METHOD("get"), &RxCurrentThreadScheduler::get);
        ClassDB::bind_static_method("RxCurrentThreadScheduler", D_METHOD("singleton"), &RxCurrentThreadScheduler::singleton);
        RX_WRAPPER_CAST_BINDS(RxCurrentThreadScheduler)
    }

public:
    inline static Ref<RxCurrentThreadScheduler> get() {
        return RxCurrentThreadScheduler::wrap(CurrentThreadScheduler::get());
    }
    inline static Ref<RxCurrentThreadScheduler> singleton() {
        return RxCurrentThreadScheduler::wrap(CurrentThreadScheduler::singleton());
    }

}; // END class RxCurrentThreadScheduler

class RxEventLoopScheduler : public RxScheduler {
    GDCLASS(RxEventLoopScheduler, RxScheduler)
    RX_WRAPPER(RxEventLoopScheduler, EventLoopScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxEventLoopScheduler", D_METHOD("get", "exit_if_empty"), &RxEventLoopScheduler::get, DEFVAL(false));
        ClassDB::bind_method(D_METHOD("dispose"), &RxEventLoopScheduler::dispose);
        ClassDB::bind_method(D_METHOD("run"), &RxEventLoopScheduler::run);
        RX_WRAPPER_CAST_BINDS(RxEventLoopScheduler)
    }

public:
    inline static Ref<RxEventLoopScheduler> get(bool exit_if_empty = false) {
        return RxEventLoopScheduler::wrap(EventLoopScheduler::get(default_thread_factory, exit_if_empty));
    }

    inline void dispose() { this->_ptr->dispose(); }
    inline void run() { this->_ptr->run(); }

}; // END class RxEventLoopScheduler

class RxImmediateScheduler : public RxScheduler {
    GDCLASS(RxImmediateScheduler, RxScheduler)
    RX_WRAPPER(RxImmediateScheduler, ImmediateScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxImmediateScheduler", D_METHOD("get"), &RxImmediateScheduler::get);
        ClassDB::bind_static_method("RxImmediateScheduler", D_METHOD("singleton"), &RxImmediateScheduler::singleton);
        RX_WRAPPER_CAST_BINDS(RxImmediateScheduler)
    }

public:
    inline static Ref<RxImmediateScheduler> get() {
        return RxImmediateScheduler::wrap(ImmediateScheduler::get());
    }
    inline static Ref<RxImmediateScheduler> singleton() {
        return RxImmediateScheduler::wrap(ImmediateScheduler::singleton());
    }

}; // END class RxImmediateScheduler

class RxNewThreadScheduler : public RxScheduler {
    GDCLASS(RxNewThreadScheduler, RxScheduler)
    RX_WRAPPER(RxNewThreadScheduler, NewThreadScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNewThreadScheduler", D_METHOD("get"), &RxNewThreadScheduler::get);
        RX_WRAPPER_CAST_BINDS(RxNewThreadScheduler)
    }

public:
    inline static Ref<RxNewThreadScheduler> get() {
        return RxNewThreadScheduler::wrap(NewThreadScheduler::get());
    }

}; // END class RxNewThreadScheduler


class RxSceneTreeTimeoutScheduler : public RxScheduler {
    GDCLASS(RxSceneTreeTimeoutScheduler, RxScheduler)
    RX_WRAPPER(RxSceneTreeTimeoutScheduler, SceneTreeTimeoutScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxSceneTreeTimeoutScheduler", D_METHOD("get", "process_always", "process_in_physics", "ignore_time_scale"), &RxSceneTreeTimeoutScheduler::get, DEFVAL(true), DEFVAL(false), DEFVAL(false));
        ClassDB::bind_static_method("RxSceneTreeTimeoutScheduler", D_METHOD("singleton", "process_always", "process_in_physics", "ignore_time_scale"), &RxSceneTreeTimeoutScheduler::singleton, DEFVAL(true), DEFVAL(false), DEFVAL(false));
        RX_WRAPPER_CAST_BINDS(RxSceneTreeTimeoutScheduler)
    }

public:
    inline static Ref<RxSceneTreeTimeoutScheduler> get(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false) {
        return RxSceneTreeTimeoutScheduler::wrap(SceneTreeTimeoutScheduler::get(process_always, process_in_physics, ignore_time_scale));
    }
    inline static Ref<RxSceneTreeTimeoutScheduler> singleton(bool process_always = true, bool process_in_physics = false, bool ignore_time_scale = false) {
        return RxSceneTreeTimeoutScheduler::wrap(SceneTreeTimeoutScheduler::singleton(process_always, process_in_physics, ignore_time_scale));
    }

}; // END class RxSceneTreeTimeoutScheduler

class RxTimeoutScheduler : public RxScheduler {
    GDCLASS(RxTimeoutScheduler, RxScheduler)
    RX_WRAPPER(RxTimeoutScheduler, TimeoutScheduler, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxTimeoutScheduler", D_METHOD("get"), &RxTimeoutScheduler::get);
        ClassDB::bind_static_method("RxTimeoutScheduler", D_METHOD("singleton"), &RxTimeoutScheduler::singleton);
        RX_WRAPPER_CAST_BINDS(RxTimeoutScheduler)
    }

public:
    inline static Ref<RxTimeoutScheduler> get() {
        return RxTimeoutScheduler::wrap(TimeoutScheduler::get());
    }
    inline static Ref<RxTimeoutScheduler> singleton() {
        return RxTimeoutScheduler::wrap(TimeoutScheduler::singleton());
    }

}; // END class RxTimeoutScheduler

class RxGodotSignalScheduler : public RxGodotSignalSchedulerBase {
    GDCLASS(RxGodotSignalScheduler, RxGodotSignalSchedulerBase)
    RX_WRAPPER(RxGodotSignalScheduler, GodotSignalScheduler, RxGodotSignalSchedulerBase, GodotSignalSchedulerBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxGodotSignalScheduler", D_METHOD("get"), &RxGodotSignalScheduler::get);
        ClassDB::bind_static_method("RxGodotSignalScheduler", D_METHOD("singleton"), &RxGodotSignalScheduler::singleton);
        RX_WRAPPER_CAST_BINDS(RxGodotSignalScheduler)
    }

public:
    inline static Ref<RxGodotSignalScheduler> get() {
        return RxGodotSignalScheduler::wrap(GodotSignalScheduler::get());
    }
    inline static Ref<RxGodotSignalScheduler> singleton() {
        return RxGodotSignalScheduler::wrap(GodotSignalScheduler::singleton());
    }

}; // END class RxGodotSignalScheduler

class RxScheduler_ : public RxScheduler {
    GDCLASS(RxScheduler_, RxScheduler)
    _RX_WRAPPER(RxScheduler_, Scheduler_, RxScheduler, Scheduler)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxScheduler_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxScheduler_::_template);
    }

public:
    RxScheduler_() 
        : RxScheduler(std::static_pointer_cast<Scheduler>(Scheduler_::get())), 
        _ptr(std::static_pointer_cast<Scheduler_>(RxScheduler::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxSchedulerTemplate> t) {
        this->_ptr->_template(t);
    }
    

}; // END class RxScheduler_

} // END namespace wrappers
} // END namespace rx

#endif // RX_WRAPPER_SCHEDULER_H