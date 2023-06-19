#include "catchscheduler.h"

#include "internal/lambda.h"
#include "exception/exception.h"
#include "exception/exceptionwrapper.h"
#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"

void CatchScheduler::_bind_methods() {
    ClassDB::bind_static_method("CatchScheduler", D_METHOD("Get", "scheduler", "handler"), &CatchScheduler::Get);
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &CatchScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &CatchScheduler::schedule_absolute, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &CatchScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_periodic", "period", "action", "state"), &CatchScheduler::schedule_periodic, DEFVAL(Variant()));

    ClassDB::bind_method(D_METHOD("now"), &CatchScheduler::now);
    ClassDB::bind_method(D_METHOD("_clone", "scheduler"), &CatchScheduler::_clone);
    ClassDB::bind_method(D_METHOD("_wrap", "action"), &CatchScheduler::_wrap);
    ClassDB::bind_method(D_METHOD("_get_recursive_wrapper", "scheduler"), &CatchScheduler::_get_recursive_wrapper);
}

Ref<CatchScheduler> CatchScheduler::Get(Ref<SchedulerBase> scheduler, const Callable& handler) {
    return memnew(CatchScheduler(scheduler, handler));
}

Ref<AbsoluteTime> CatchScheduler::now() {
    return this->_scheduler->now();
}

Ref<DisposableBase> CatchScheduler::schedule(const Callable& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule(action_, state);
}

Ref<DisposableBase> CatchScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule_absolute(duetime, action_, state);
}

Ref<DisposableBase> CatchScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto action_ = this->_wrap(action);
    return this->_scheduler->schedule_relative(duetime, action_, state);
}

Ref<DisposableBase> CatchScheduler::schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state) {
    Callable _schedule_periodic;
    if (!this->_scheduler->has_method("schedule_periodic")) {
        throw NotImplementedException();
    }
    _schedule_periodic = Callable(*(this->_scheduler), "schedule_periodic");

    auto disp = SingleAssignmentDisposable::Get();
    bool failed = false;

    Array nonlocals = Array::make(failed);
    Callable periodic = Lambda(VARIANT_FUN1([=](const Variant& state = Variant()){
        auto _nonlocals = nonlocals; bool _failed = _nonlocals[0];
        auto _disp = disp;

        if (_failed) {
            return Variant();
        }
        try {
            return action.callv(Array::make(state));
        }
        catch(std::exception ex_) {
            Ref<RxError> ex = RxError::wrap(ex_);
            _failed = true;
            if (!this->_handler.callv(Array::make(ex))) {
                ex->raise();
            }
            _disp->dispose();
            return Variant();
        }

        _nonlocals[0] = _failed;
    }));

    auto periodic_scheduler = REF_CAST(*(this->_scheduler), PeriodicScheduler);
    disp->set_disposable(periodic_scheduler->schedule_periodic(period, periodic, state));
    return disp;
}

Ref<CatchScheduler> CatchScheduler::_clone(Ref<SchedulerBase> scheduler) const {
    return CatchScheduler::Get(scheduler, this->_handler);
}

Callable CatchScheduler::_wrap(const Callable& action) const {
    Ref<CatchScheduler> parent(this);
    Ref<CatchScheduler> self(this);

    Callable wrapped_action = Lambda(VARIANT_FUN2([=](Ref<SchedulerBase> self, const Variant& state = Variant()) -> Ref<DisposableBase> {
        auto _self = self; auto _parent = parent;
        try {
            return action.callv(Array::make(_parent->_get_recursive_wrapper(_self), state));
        }
        catch (std::exception ex_) {
            Ref<RxError> ex = RxError::wrap(ex_);
            if (!this->_handler.callv(Array::make(ex))) {
                ex->raise();
            }
            return Disposable::Get();
        }
    }));

    return wrapped_action;
}

Ref<CatchScheduler> CatchScheduler::_get_recursive_wrapper(Ref<SchedulerBase> scheduler) {
    if (this->_recursive_wrapper.is_null() || this->_recursive_original != scheduler) {
        this->_recursive_original = scheduler;
        auto wrapper = this->_clone(scheduler);
        wrapper->_recursive_original = scheduler;
        wrapper->_recursive_wrapper = wrapper;
        this->_recursive_wrapper = wrapper;
    }

    return this->_recursive_wrapper;
}