#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/wrapper.h"

#include "wrapper/observer.h"
#include "wrapper/scheduler.h"

#include "abstract/observable.h"
#include "abstract/observer.h"
#include "exception/exception.h"
#include "exception/exceptionwrapper.h"

#include "cast.h"
#include "basic.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxObservable : public RefCounted {
    GDCLASS(RxObservable, RefCounted)
    RX_ABSTRACT_WRAPPER(RxObservable, ObservableBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxObservable::equals);
        {
		    MethodInfo mi;
		    mi.arguments.push_back(PropertyInfo(Variant::NIL, "subscription_args"));
		    mi.name = "subscribe";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "subscribe", &RxObservable::subscribe, mi);
	    }
    }
public:
    inline Ref<RxDisposable> subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
        Variant _args[4];
        for (auto i = 0; i < 4; i++) {
            _args[i] = arg_count > i ? *(args[i]) : Variant();
        }
        if (auto _obv = DYN_CAST_OR_NULL(_args[0], RxObserver)) {
            auto obv = _obv->unwrap();
            if (auto _s = DYN_CAST_OR_NULL(_args[1], RxScheduler)) {
                auto s = _s->unwrap();
                return RxDisposable::wrap(_ptr->subscribe(obv, s));
            }
            return RxDisposable::wrap(_ptr->subscribe(obv));
        }
        on_next_t on_next_ = IS_TYPE(_args[0], CALLABLE) ? on_next_cb(_args[0]) : DEFAULT_ON_NEXT;
        on_error_t on_error_ = IS_TYPE(_args[1], CALLABLE) ? on_error_cb(_args[1]) : DEFAULT_ON_ERROR;
        on_completed_t on_completed_ = IS_TYPE(_args[2], CALLABLE) ? on_completed_cb(_args[2]) : DEFAULT_ON_COMPLETED;
        Ref<RxScheduler> scheduler;
        for (auto i = 0; i < arg_count; i++) {
            if (auto _s = DYN_CAST_OR_NULL(_args[i], RxScheduler)) {
                scheduler = Ref<RxScheduler>(_s);
                break;
            }
        }
        return RxDisposable::wrap(this->_ptr->subscribe(on_next_, on_error_, on_completed_, scheduler->unwrap()));
    }
};

} // END namespace wrapper

using namespace rx::wrappers;

static subscription_t subscription_cb(const Callable& cb) {
    return subscription_t([cb](const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler) {
        Ref<RxDisposable> disp = cb.callv(Array::make(RxObserver::wrap(observer), RxScheduler::wrap(scheduler)));
        return disp.is_null() ? nullptr : disp->unwrap();
    });
}


} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H