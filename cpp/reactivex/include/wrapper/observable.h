#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/wrapper.h"

#include "wrapper/observer.h"
#include "wrapper/scheduler.h"

#include "abstract/observable.h"
#include "abstract/observer.h"
#include "observable/observable.h"
#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

#include "observable/definitions.h"

#include "cast.h"
#include "basic.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::observable;

namespace rx {

static subscription_t subscription_cb(const Callable& cb);
static observable_factory_t observable_factory_cb(const Callable& cb);

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
    }
public:
    Ref<RxDisposableBase> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
};

class RxObservable : public RxObservableBase {
    GDCLASS(RxObservable, RxObservableBase)
    RX_WRAPPER(RxObservable, Observable, RxObservableBase, ObservableBase)

public:
#include "observable/definitions.inc"

protected:
    static inline void _bind_methods() {
        OBSERVABLE_CONSTRUCTORS_BINDS
    }
};

} // END namespace wrapper

using namespace rx::wrappers;

subscription_t subscription_cb(const Callable& cb) {
    return subscription_t([cb](const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler) {
        Ref<RxDisposableBase> disp = cb.callv(Array::make(RxObserverBase::wrap(observer), RxSchedulerBase::wrap(scheduler)));
        return RxDisposableBase::unwrap(disp);
    });
}

observable_factory_t observable_factory_cb(const Callable& cb) {
    return [cb](const std::shared_ptr<SchedulerBase>& scheduler) -> std::shared_ptr<Observable> {
        return RxObservable::unwrap(cb.callv(Array::make(RxSchedulerBase::wrap(scheduler))));
    };
}

} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H