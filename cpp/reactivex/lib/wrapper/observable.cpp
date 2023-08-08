#include "wrapper/observable.h"

namespace rx::wrappers {

Ref<RxDisposable> RxObservable::subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    Variant _args[4];
    for (auto i = 0; i < 4; i++) {
        _args[i] = arg_count > i ? *(args[i]) : Variant();
    }
    if (auto _obv = DYN_CAST_OR_NULL(_args[0], RxObserver)) {
        auto obv = _obv->getptr();
        if (auto _s = DYN_CAST_OR_NULL(_args[1], RxScheduler)) {
            auto s = _s->getptr();
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
    return RxDisposable::wrap(this->_ptr->subscribe(on_next_, on_error_, on_completed_, RxScheduler::unwrap(scheduler)));
}

} // END namespace rx::wrappers