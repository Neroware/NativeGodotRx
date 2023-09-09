#include "wrapper/abstract.h"

#include "disposable/autodisposer.h"

using namespace rx::disposable;

namespace rx::wrappers {


void RxDisposableBase::dispose() { this->_ptr->dispose(); }
void RxDisposableBase::dispose_with(Object* obj) { RxAutoDisposer::add_to(obj, this->_ptr); }


Ref<RxIteratorBase> RxIterableBase::iter() { 
    return RxIteratorBase::wrap(this->_ptr->iter()); 
}


Variant RxIteratorBase::next() {
    return this->_ptr->next();
}
bool RxIteratorBase::has_next() {
    return this->_ptr->has_next();
}
void RxIteratorBase::foreach(const Callable& what) {
    while (this->has_next()) {
        Variant res = what.callv(Array::make(this->next()));
        if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
            break;
    }
}
void RxIteratorBase::enumerate(const Callable& what) {
    uint64_t i = 0;
    while (this->has_next()) {
        Variant res = what.callv(Array::make(i++, this->next()));
        if (res.get_type() == Variant::Type::BOOL && !res.booleanize())
            break;
    }
}


void RxLockBase::lock() { 
    this->_ptr->lock(); 
}
void RxLockBase::unlock() { 
    this->_ptr->unlock(); 
}
bool RxLockBase::try_lock() { 
    return this->_ptr->try_lock(); 
}


Ref<RxDisposableBase> RxObservableBase::subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    Variant _args[4];
    for (auto i = 0; i < 4; i++) {
        _args[i] = arg_count > i ? *(args[i]) : Variant();
    }
    if (auto _obv = DYN_CAST_OR_NULL(_args[0], RxObserverBase)) {
        auto obv = _obv->getptr();
        if (auto _s = DYN_CAST_OR_NULL(_args[1], RxSchedulerBase)) {
            auto s = _s->getptr();
            return RxDisposableBase::wrap(_ptr->subscribe(obv, s));
        }
        return RxDisposableBase::wrap(_ptr->subscribe(obv));
    }
    on_next_t on_next_ = IS_TYPE(_args[0], CALLABLE) ? on_next_cb(_args[0]) : DEFAULT_ON_NEXT;
    on_error_t on_error_ = IS_TYPE(_args[1], CALLABLE) ? on_error_cb(_args[1]) : DEFAULT_ON_ERROR;
    on_completed_t on_completed_ = IS_TYPE(_args[2], CALLABLE) ? on_completed_cb(_args[2]) : DEFAULT_ON_COMPLETED;
    Ref<RxSchedulerBase> scheduler;
    for (auto i = 0; i < arg_count; i++) {
        if (auto _s = DYN_CAST_OR_NULL(_args[i], RxSchedulerBase)) {
            scheduler = Ref<RxSchedulerBase>(_s);
            break;
        }
    }
    return RxDisposableBase::wrap(this->_ptr->subscribe(on_next_, on_error_, on_completed_, RxSchedulerBase::unwrap(scheduler)));
}


void RxObserverBase::on_next(const Variant& item) {
    this->_ptr->on_next(item);
}
void RxObserverBase::on_error(Ref<RxError> error) {
    this->_ptr->on_error(RxError::unwrap(error));
}
void RxObserverBase::on_completed() {
    this->_ptr->on_completed();
}


double RxSchedulerBase::to_seconds(const Variant& value) {
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t->to_sec();
    }
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return dt->to_sec();
    }
    return value.operator double();
}
Ref<AbsoluteTime> RxSchedulerBase::to_datetime(const Variant& value) {
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t;
    }
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return memnew(AbsoluteTime(UTC_ZERO + dt->dt));
    }
    return AbsoluteTime::from_seconds(value.operator double());
}
Ref<RelativeTime> RxSchedulerBase::to_timedelta(const Variant& value) {
    if (auto dt = DYN_CAST_OR_NULL(value, RelativeTime)) {
        return dt;
    }
    if (auto t = DYN_CAST_OR_NULL(value, AbsoluteTime)) {
        return t->time_since_epoch();
    }
    return RelativeTime::from_seconds(value.operator double());
}
Ref<RxDisposableBase> RxSchedulerBase::schedule(const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule(scheduled_action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule_absolute(duetime->t, scheduled_action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule_relative(duetime->dt, scheduled_action_cb(action), state));
}
Ref<RxDisposableBase> RxSchedulerBase::schedule_periodic(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto periodic_scheduler = std::dynamic_pointer_cast<PeriodicSchedulerBase>(this->_ptr);
    if (!periodic_scheduler) {
        throw NotImplementedException();
    }
    return RxDisposableBase::wrap(periodic_scheduler->schedule_periodic(duetime->dt, periodic_action_cb(action), state));
}
Ref<AbsoluteTime> RxSchedulerBase::now() {
    return memnew(AbsoluteTime(this->_ptr->now()));
}
Ref<RxDisposableBase> RxSchedulerBase::invoke_action(const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->invoke_action(scheduled_action_cb(action), state));
}


Ref<RxDisposableBase> RxPeriodicSchedulerBase::schedule_periodic(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    return RxDisposableBase::wrap(this->_ptr->schedule_periodic(duetime->dt, periodic_action_cb(action), state));
}


void RxStartableBase::start() { this->_ptr->start(); }
void RxStartableBase::await() { this->_ptr->await(); }


Ref<RxDisposableBase> RxSubjectBase::subscribe(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    Variant _args[4];
    for (auto i = 0; i < 4; i++) {
        _args[i] = arg_count > i ? *(args[i]) : Variant();
    }
    if (auto _obv = DYN_CAST_OR_NULL(_args[0], RxObserverBase)) {
        auto obv = _obv->getptr();
        if (auto _s = DYN_CAST_OR_NULL(_args[1], RxSchedulerBase)) {
            auto s = _s->getptr();
            return RxDisposableBase::wrap(_ptr->subscribe(obv, s));
        }
        return RxDisposableBase::wrap(_ptr->subscribe(obv));
    }
    on_next_t on_next_ = IS_TYPE(_args[0], CALLABLE) ? on_next_cb(_args[0]) : DEFAULT_ON_NEXT;
    on_error_t on_error_ = IS_TYPE(_args[1], CALLABLE) ? on_error_cb(_args[1]) : DEFAULT_ON_ERROR;
    on_completed_t on_completed_ = IS_TYPE(_args[2], CALLABLE) ? on_completed_cb(_args[2]) : DEFAULT_ON_COMPLETED;
    Ref<RxSchedulerBase> scheduler;
    for (auto i = 0; i < arg_count; i++) {
        if (auto _s = DYN_CAST_OR_NULL(_args[i], RxSchedulerBase)) {
            scheduler = Ref<RxSchedulerBase>(_s);
            break;
        }
    }
    return RxDisposableBase::wrap(this->_ptr->subscribe(on_next_, on_error_, on_completed_, RxSchedulerBase::unwrap(scheduler)));
}
void RxSubjectBase::on_next(const Variant& item) {
    this->_ptr->on_next(item);
}
void RxSubjectBase::on_error(Ref<RxError> error) {
    this->_ptr->on_error(RxError::unwrap(error));
}
void RxSubjectBase::on_completed() {
    this->_ptr->on_completed();
}

} // END namespace rx::wrappers

