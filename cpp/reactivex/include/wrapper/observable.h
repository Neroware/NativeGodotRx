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

class RxObservable : public RefCounted {
    GDCLASS(RxObservable, RefCounted)
    RX_ABSTRACT_WRAPPER(RxObservable, ObservableBase)

protected:
    static inline void _bind_methods() {
        OBSERVABLE_CONSTRUCTORS_BINDS
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObservable::equals);
        {
		    MethodInfo mi;
		    mi.name = "subscribe";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "subscribe", &RxObservable::subscribe, mi);
	    }
    }
public:
    Ref<RxDisposable> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
#include "observable/definitions.inc"
};

} // END namespace wrapper

using namespace rx::wrappers;

subscription_t subscription_cb(const Callable& cb) {
    return subscription_t([cb](const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler) {
        Ref<RxDisposable> disp = cb.callv(Array::make(RxObserver::wrap(observer), RxScheduler::wrap(scheduler)));
        return RxDisposable::unwrap(disp);
    });
}

observable_factory_t observable_factory_cb(const Callable& cb) {
    return [cb](const std::shared_ptr<SchedulerBase>& scheduler) -> std::shared_ptr<Observable> {
        return std::static_pointer_cast<Observable>(RxObservable::unwrap(cb.callv(Array::make(RxScheduler::wrap(scheduler)))));
    };
}

} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H