#ifndef RX_WRAPPER_ABSTRACT_H
#define RX_WRAPPER_ABSTRACT_H

#include "wrapper/wrapper.h"

#include "abstract/disposable.h"
#include "abstract/iterator.h"
#include "abstract/iterable.h"
#include "abstract/lock.h"
#include "abstract/observable.h"
#include "abstract/observer.h"
#include "abstract/periodicscheduler.h"
#include "abstract/scheduler.h"
#include "abstract/startable.h"
#include "abstract/subject.h"

#include "internal/time.h"
#include "internal/utils.h"

#include "cast.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxLockBase : public RefCounted {
    GDCLASS(RxLockBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxLockBase, LockBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("lock"), &RxLockBase::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxLockBase::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxLockBase::try_lock);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxLockBase::equals);
        RX_WRAPPER_CAST_BINDS(RxLockBase)
    }
public:
    void lock();
    void unlock();
    bool try_lock();
};

} // END namespace rx::wrappers

namespace rx {
namespace wrappers {

class RxDisposableBase : public RefCounted {
    GDCLASS(RxDisposableBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxDisposableBase, DisposableBase)
    
protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("dispose"), &RxDisposableBase::dispose);
        ClassDB::bind_method(D_METHOD("dispose_with", "obj"), &RxDisposableBase::dispose_with);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxDisposableBase::equals);
        RX_WRAPPER_CAST_BINDS(RxDisposableBase)
    }
public:
    void dispose();
    void dispose_with(Object* obj);
};

} // END namespace wrapper
} // END namespace rx

namespace rx::wrappers {

class RxIteratorBase : public RefCounted {
    GDCLASS(RxIteratorBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxIteratorBase, IteratorBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("next"), &RxIteratorBase::next);
        ClassDB::bind_method(D_METHOD("has_next"), &RxIteratorBase::has_next);
        ClassDB::bind_method(D_METHOD("foreach", "cb"), &RxIteratorBase::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "cb"), &RxIteratorBase::enumerate);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIteratorBase::equals);
        RX_WRAPPER_CAST_BINDS(RxIteratorBase)
    }

public:
    Variant next();
    bool has_next();

    void foreach(const Callable& what);
    void enumerate(const Callable& what);
};

class RxIterableBase : public RefCounted {
    GDCLASS(RxIterableBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxIterableBase, IterableBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("iter"), &RxIterableBase::iter);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxIterableBase::equals);
        RX_WRAPPER_CAST_BINDS(RxIterableBase)
    }
public:
    Ref<RxIteratorBase> iter();
};

} // END namespace rx::wrappers

namespace rx {
namespace wrappers {

class RxObserverBase : public RefCounted {
    GDCLASS(RxObserverBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxObserverBase, ObserverBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("on_next", "item"), &RxObserverBase::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "error"), &RxObserverBase::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &RxObserverBase::on_completed);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObserverBase::equals);
        RX_WRAPPER_CAST_BINDS(RxObserverBase)
    }
public:
    void on_next(const Variant& item);
    void on_error(Ref<RxError> error);
    void on_completed();
};

} // END namespace wrapper
} // END namespace rx

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
        RX_WRAPPER_CAST_BINDS(RxSchedulerBase)
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

} // END namespace wrappers
} // END namespace rx

namespace rx {
namespace wrappers {

class RxPeriodicSchedulerBase : public RefCounted {
    GDCLASS(RxPeriodicSchedulerBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxPeriodicSchedulerBase, PeriodicSchedulerBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("schedule_periodic", "duetime", "action", "state"), &RxPeriodicSchedulerBase::schedule_periodic, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxPeriodicSchedulerBase::equals);
        RX_WRAPPER_CAST_BINDS(RxPeriodicSchedulerBase)
    }
public:
    Ref<RxDisposableBase> schedule_periodic(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant());

}; // END class RxPeriodicSchedulerBase

} // END namespace wrappers
} // END namespace rx

namespace rx {
namespace wrappers {

class RxStartableBase : public RefCounted {
    GDCLASS(RxStartableBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxStartableBase, StartableBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("start"), &RxStartableBase::start);
        ClassDB::bind_method(D_METHOD("await"), &RxStartableBase::await);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxStartableBase::equals);
        RX_WRAPPER_CAST_BINDS(RxStartableBase)
    }
public:
    void start();
    void await();

}; // END class RxPeriodicSchedulerBase

} // END namespace wrappers
} // END namespace rx

namespace rx {
namespace wrappers {

class RxObservableBase : public RefCounted {
    GDCLASS(RxObservableBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxObservableBase, ObservableBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObservableBase::equals);
        {
		    MethodInfo mi;
		    mi.name = "subscribe";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "subscribe", &RxObservableBase::subscribe, mi);
	    }
        RX_WRAPPER_CAST_BINDS(RxObservableBase)
    }
public:
    Ref<RxDisposableBase> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
};

} // END namespace wrappers
} // END namespace rx

namespace rx {
namespace wrappers {

class RxSubjectBase : public RefCounted {
    GDCLASS(RxSubjectBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxSubjectBase, SubjectBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxSubjectBase::equals);
        {
		    MethodInfo mi;
		    mi.name = "subscribe";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "subscribe", &RxSubjectBase::subscribe, mi);
	    }
        ClassDB::bind_method(D_METHOD("on_next", "item"), &RxSubjectBase::on_next);
        ClassDB::bind_method(D_METHOD("on_error", "error"), &RxSubjectBase::on_error);
        ClassDB::bind_method(D_METHOD("on_completed"), &RxSubjectBase::on_completed);
        RX_WRAPPER_CAST_BINDS(RxSubjectBase)
    }
public:
    Ref<RxDisposableBase> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
    void on_next(const Variant& item);
    void on_error(Ref<RxError> error);
    void on_completed();

};

} // END namespace wrapper
} // END namespace rx

#endif // RX_WRAPPER_ABSTRACT_H