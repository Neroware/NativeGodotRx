#include "wrapper/observable.h"

namespace rx::wrappers {

#define UNWRAP_VARIADICS(argv) TypedArray<RxObservable> argv; for (auto argi = 0ul; argi < arg_count; argi++) argv.push_back(*(args[argi])); 
#define UNWRAP_VARIADICS_AND_FORWARD(method) UNWRAP_VARIADICS(sources) return method(sources);
#define VARIANT_ITERABLE(sources) rx_wrapper_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
#define BIND_VARARG_METHOD(Cls, method_name, arg_type, arg_name) { \
        MethodInfo mi; \
		mi.arguments.push_back(PropertyInfo(Variant::arg_type, #arg_name)); \
		mi.name = #method_name; \
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, #method_name, &Cls::method_name, mi); \
    }


static time_delta_t variant_to_timedelta(const Variant& time) {
    if (IS_NUMBER(time)) {
        double time_ = time;
        return Scheduler::to_timedelta(time_);
    }
    if (auto t = DYN_CAST_OR_NULL(time, AbsoluteTime)) {
        return Scheduler::to_timedelta(t->t);
    }
    if (auto dt = DYN_CAST_OR_NULL(time, RelativeTime)) {
        return dt->dt;
    }
    throw BadArgumentException("The provided argument was not a timespan!");
} 

/* WRAPPER METHODS FOR ENGINE INTERFACE */

// case.h
Ref<RxObservable> RxObservable::case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source) {
    return RxObservable::wrap(Observables::case_mapper(
        std::function<Variant()>([mapper](){ return mapper.callv(Array()); }),
        dictionary_mapping<RxObservable, Observable>(sources),
        RxObservable::unwrap(default_source)
    ));
}

// catch.h
Ref<RxObservable> RxObservable::catch_with_iterable(const Variant& sources) {
    return RxObservable::wrap(Observables::catch_with_iterable(
        VARIANT_ITERABLE(sources)
    ));
}

// combinelatest.h
Ref<RxObservable> RxObservable::combine_latest(const Variant& sources) {
    return RxObservable::wrap(Observables::combine_latest(
        VARIANT_ITERABLE(sources)
    ));
}

// concat.h
Ref<RxObservable> RxObservable::concat(const Variant& sources) {
    return RxObservable::wrap(Observables::concat_with_iterable(
        VARIANT_ITERABLE(sources)
    ));
}

// defer.h
Ref<RxObservable> RxObservable::defer(const Callable& factory) {
    return RxObservable::wrap(Observables::defer(observable_factory_cb(factory)));
}

// empty.h
Ref<RxObservable> RxObservable::empty(Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::empty(RxSchedulerBase::unwrap(scheduler)));
}

// forkjoin.h
Ref<RxObservable> RxObservable::fork_join(const Variant& sources) {
    return RxObservable::wrap(Observables::fork_join(
        VARIANT_ITERABLE(sources)
    ));
}

// fromiterable.h
Ref<RxObservable> RxObservable::from_iterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::from_iterable(
        RxIterableBase::unwrap(rx::iterator::to_iterable(iterable)),
        RxSchedulerBase::unwrap(scheduler)
    ));
}
Ref<RxObservable> RxObservable::from(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::from_iterable(
        RxIterableBase::unwrap(rx::iterator::to_iterable(iterable)),
        RxSchedulerBase::unwrap(scheduler)
    ));
}

// generate.h
Ref<RxObservable> RxObservable::generate(const Variant& initial_state, const Callable& condition, const Callable& iterate) {
    return RxObservable::wrap(Observables::generate(
        initial_state,
        [condition](const Variant& state) -> bool { return condition.callv(Array::make(state)); },
        [iterate](const Variant& state) -> Variant { return iterate.callv(Array::make(state)); }
    ));
}

// ifthen.h
Ref<RxObservable> RxObservable::if_then(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source) {
    return RxObservable::wrap(Observables::if_then(
        [condition]() -> bool { return condition.callv(Array()); },
        RxObservable::unwrap(then_source),
        RxObservable::unwrap(else_source)
    ));
}

// never.h
Ref<RxObservable> RxObservable::never() {
    return RxObservable::wrap(Observables::never());
}

// onerrorresumenext.h
Ref<RxObservable> RxObservable::on_error_resume_next(const Variant& sources, bool use_factory) {
    if (use_factory) {
        return RxObservable::wrap(Observables::on_error_resume_next(
            rx_mapper_iterable<std::function<std::shared_ptr<Observable>(const std::exception_ptr&)>, Variant::Type::CALLABLE>(
                RxIterableBase::unwrap(rx::iterator::to_iterable(sources)),
                [](const Callable& cb) {
                    return [cb](const std::exception_ptr& e) -> std::shared_ptr<Observable> {
                        return RxObservable::unwrap(cb.callv(Array::make(RxError::wrap(e))));
                    };
                }
            )
        ));
    }
    return RxObservable::wrap(Observables::on_error_resume_next(
        rx_wrapper_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
    ));
}

// range.h
Ref<RxObservable> RxObservable::range(int64_t start, int64_t stop, int64_t step) {
    return RxObservable::wrap(Observables::range(start, stop, step));
}

// returnvalue.h
Ref<RxObservable> RxObservable::return_value(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::return_value(value, RxSchedulerBase::unwrap(scheduler)));
}
Ref<RxObservable> RxObservable::just(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::return_value(value, RxSchedulerBase::unwrap(scheduler)));
}

// throw.h
Ref<RxObservable> RxObservable::throw_message(const String& what, const String& type, Ref<RxSchedulerBase> scheduler) {
    try { throw rx_exception(what.ascii().get_data(), type.ascii().get_data()); } catch(...) {
        return RxObservable::wrap(Observables::throw_error(std::current_exception(), RxSchedulerBase::unwrap(scheduler)));
    }
}
Ref<RxObservable> RxObservable::throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(Observables::throw_error(RxError::unwrap(error), RxSchedulerBase::unwrap(scheduler)));
}

// timer.h
Ref<RxObservable> RxObservable::timer(const Variant& duetime, const Variant& period, Ref<RxSchedulerBase> scheduler) {
    bool periodic = false;
    time_delta_t period_;
    if (auto _period = DYN_CAST_OR_NULL(period, RelativeTime)) {
        period_ = _period->dt;
        periodic = true;
    }
    else if (IS_NUMBER(period)) {
        double period_dt = period;
        period_ = Scheduler::to_timedelta(period_dt);
        periodic = true;
    }
    if (auto _duetime = DYN_CAST_OR_NULL(duetime, AbsoluteTime)) {
        return periodic ?
            RxObservable::wrap(Observables::periodic_timer(_duetime->t, period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer(_duetime->t, RxSchedulerBase::unwrap(scheduler)));
    }
    if (auto _duetime = DYN_CAST_OR_NULL(duetime, RelativeTime)) {
        return periodic ?
            RxObservable::wrap(Observables::periodic_timer(_duetime->dt, period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer(_duetime->dt, RxSchedulerBase::unwrap(scheduler)));
    }
    if (IS_NUMBER(duetime)) {
        double duetime_ = duetime;
        return periodic ?
            RxObservable::wrap(Observables::periodic_timer(Scheduler::to_timedelta(duetime_), period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer(Scheduler::to_timedelta(duetime_), RxSchedulerBase::unwrap(scheduler)));
    }
    throw BadArgumentException("No timespan type fits into timer observable constructor!");
}
Ref<RxObservable> RxObservable::periodic_timer(const Variant& period, Ref<RxSchedulerBase> scheduler) {
    return timer(period, period, scheduler);
}

// using.h
Ref<RxObservable> RxObservable::using_resource(const Callable& resource_factory, const Callable& observable_factory) {
    return RxObservable::wrap(Observables::using_resource(
        [resource_factory]() -> std::shared_ptr<DisposableBase> {
            return RxDisposableBase::unwrap(resource_factory.callv(Array())); 
        },
        [observable_factory](const std::shared_ptr<DisposableBase>& d) -> std::shared_ptr<Observable> { 
            return RxObservable::unwrap(observable_factory.callv(Array::make(RxDisposableBase::wrap(d)))); 
        }
    ));
}

// withlatestfrom.h
Ref<RxObservable> RxObservable::with_latest_from(Ref<RxObservable> parent, const Variant& sources) {
    return RxObservable::wrap(Observables::with_latest_from(RxObservable::unwrap(parent), VARIANT_ITERABLE(sources)));
}

// zip.h
Ref<RxObservable> RxObservable::zip(const Variant& sources) {
    return RxObservable::wrap(Observables::zip(
        VARIANT_ITERABLE(sources)
    ));
}


/* OPERATORS HERE */

// _all.h
inline Ref<RxObservable> RxObservable::all(const Callable& predicate) {
    return RxObservable::wrap(op::Operators::all(
        predicate_cb<Variant>(predicate)
    )(this->_ptr));
}

// _amb.h
inline Ref<RxObservable> RxObservable::amb(Ref<RxObservable> right_source) {
    return RxObservable::wrap(op::Operators::amb(
        RxObservable::unwrap(right_source)
    )(this->_ptr));
}

// _asobservable.h
Ref<RxObservable> RxObservable::as_observable() {
    return RxObservable::wrap(op::Operators::as_observable()(this->_ptr));
}

// _average.h
Ref<RxObservable> RxObservable::average(const Callable& key_mapper) {
    if (key_mapper.is_null()) {
        return RxObservable::wrap(op::Operators::average()(this->_ptr));
    }
    return RxObservable::wrap(op::Operators::average(
        [key_mapper](const Variant& x) -> double {
            return key_mapper.callv(Array::make(x));
        }
    )(this->_ptr));
}

// _catch.h
Ref<RxObservable> RxObservable::catch_with_handler(const Variant& handler) {
    if (auto obs = DYN_CAST_OR_NULL(handler, RxObservable)) {
        return RxObservable::wrap(op::Operators::catch_with_handler(
            RxObservable::unwrap(obs)
        )(this->_ptr));
    }
    if (IS_TYPE(handler, CALLABLE)) {
        Callable _handler = handler;
        return RxObservable::wrap(op::Operators::catch_with_handler(
            [_handler](const std::exception_ptr& error, const std::shared_ptr<Observable> observable) -> std::shared_ptr<Observable> {
                return RxObservable::unwrap(_handler.callv(Array::make(
                    RxError::wrap(error),
                    RxObservable::wrap(observable)
                )));
            }
        )(this->_ptr));
    }
    return RxObservable::wrap(this->_ptr);
}

// _combinelatest.h
Ref<RxObservable> RxObservable::combine_latest_withv(const Variant& others) {
    return RxObservable::wrap(op::Operators::combine_latest(
        VARIANT_ITERABLE(others)
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::combine_latest_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    UNWRAP_VARIADICS_AND_FORWARD(combine_latest_withv);
}


// _concat.h
Ref<RxObservable> RxObservable::concat_withv(const Variant& sources) {
    return RxObservable::wrap(op::Operators::concat(
        VARIANT_ITERABLE(sources)
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::concat_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    UNWRAP_VARIADICS_AND_FORWARD(concat_withv);
}


// _contains.h
Ref<RxObservable> RxObservable::contains(const Variant& value, const Callable& comparer) {
    return RxObservable::wrap(op::Operators::contains(
        value, comparer_cb<Variant>(comparer)
    )(this->_ptr));
}

// _count.h
Ref<RxObservable> RxObservable::count(const Callable& predicate) {
    return RxObservable::wrap(op::Operators::count(predicate_cb<Variant>(predicate))(this->_ptr));
}

// _debounce.h
Ref<RxObservable> RxObservable::debounce(const Variant& duetime, Ref<RxSchedulerBase> scheduler) {
    return RxObservable::wrap(op::Operators::debounce(
        variant_to_timedelta(duetime)
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::throttle_with_mapper(const Callable& throttle_duration_mapper) {
    return RxObservable::wrap(op::Operators::throttle_with_mapper(
        [throttle_duration_mapper](const Variant& v) -> rx_observable_t {
            return RxObservable::unwrap(throttle_duration_mapper.callv(Array::make(v)));
        }
    )(this->_ptr));
}

// _defaultifempty.h
Ref<RxObservable> RxObservable::default_if_empty(const Variant& default_value) {
    return RxObservable::wrap(op::Operators::default_if_empty(
        default_value
    )(this->_ptr));
}

// _dematerialize.h
Ref<RxObservable> RxObservable::dematerialize() {
    return RxObservable::wrap(op::Operators::dematerialize()(this->_ptr));
}

// _distinct.h
Ref<RxObservable> RxObservable::distinct(const Callable& key_mapper, const Callable& comparer) {
    return RxObservable::wrap(op::Operators::distinct(
        mapper_cb<Variant, Variant>(key_mapper),
        comparer_cb<Variant>(comparer)
    )(this->_ptr));
}

// _distinctuntilchanged.h
Ref<RxObservable> RxObservable::distinct_until_changed(const Callable& key_mapper, const Callable& comparer) {
    return RxObservable::wrap(op::Operators::distinct_until_changed(
        mapper_cb<Variant, Variant>(key_mapper),
        comparer_cb<Variant>(comparer)
    )(this->_ptr));
}

// _do.h
Ref<RxObservable> RxObservable::do_action(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    return RxObservable::wrap(op::Operators::do_action(
        on_next_cb(on_next),
        on_error_cb(on_error),
        on_completed_cb(on_completed)
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::do_observer(Ref<RxObserverBase> observer) {
    return RxObservable::wrap(op::Operators::do_observer(
        RxObserverBase::unwrap(observer)
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::do_after_next(const Callable& after_next) {
    return RxObservable::wrap(op::Operators::do_after_next(
        this->_ptr,
        on_next_cb(after_next)
    ));
}
Ref<RxObservable> RxObservable::do_on_subscribe(const Callable& on_subscribe) {
    return RxObservable::wrap(op::Operators::do_on_subscribe(
        this->_ptr,
        rx::observable::op::do_action_cb(on_subscribe)
    ));
}
Ref<RxObservable> RxObservable::do_on_dispose(const Callable& on_dispose) {
    return RxObservable::wrap(op::Operators::do_on_dispose(
        this->_ptr,
        dispose_cb(on_dispose)
    ));
}
Ref<RxObservable> RxObservable::do_on_terminate(const Callable& on_terminate) {
    return RxObservable::wrap(op::Operators::do_on_terminate(
        this->_ptr,
        rx::observable::op::do_action_cb(on_terminate)
    ));
}
Ref<RxObservable> RxObservable::do_finally(const Callable& finally_action) {
    return RxObservable::wrap(op::Operators::do_finally(
        rx::observable::op::do_action_cb(finally_action)
    )(this->_ptr));
}

// _filter.h
Ref<RxObservable> RxObservable::filter(const Callable& predicate) {
    return RxObservable::wrap(op::Operators::filter(
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}
Ref<RxObservable> RxObservable::filter_indexed(const Callable& predicate) {
    if (predicate.is_null()) {
        return RxObservable::wrap(op::Operators::filter_indexed()(this->_ptr));
    }
    return RxObservable::wrap(op::Operators::filter_indexed(
        [predicate](const Variant& v, uint64_t i) -> bool { return predicate.callv(Array::make(v, i)); }
    )(this->_ptr));
}

// _last.h
Ref<RxObservable> RxObservable::last(const Callable& predicate) {
    if (predicate.is_null()) {
        return RxObservable::wrap(op::Operators::last()(this->_ptr));
    }
    return RxObservable::wrap(op::Operators::last(
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}

// _lastordefault.h
Ref<RxObservable> RxObservable::last_or_default(const Variant& default_value, const Callable& predicate) {
    if (predicate.is_null()) {
        return RxObservable::wrap(op::Operators::last_or_default(default_value)(this->_ptr));
    }
    return RxObservable::wrap(op::Operators::last_or_default(
        default_value,
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}

// _reduce.h
Ref<RxObservable> RxObservable::reduce(const Callable& accumulator, const Variant& seed) {
    return RxObservable::wrap(op::Operators::reduce(
        accumulator_cb<Variant, Variant>(accumulator), seed
    )(this->_ptr));
}

// _map.h
Ref<RxObservable> RxObservable::map(const Callable& mapper) {
    if (mapper.is_null()) {
        return RxObservable::wrap(op::Operators::map()(this->_ptr));
    }
    return RxObservable::wrap(op::Operators::map(
        [mapper](const Variant& v) -> Variant { return mapper.callv(Array::make(v)); }
    )(this->_ptr));
}

// _scan.h
Ref<RxObservable> RxObservable::scan(const Callable& accumulator, const Variant& seed) {
    return RxObservable::wrap(op::Operators::scan(
        [accumulator](const Variant& state, const Variant& x) -> Variant { 
            return accumulator.callv(Array::make(state, x)); 
        },
        seed
    )(this->_ptr));
}

// _some.h
Ref<RxObservable> RxObservable::some(const Callable& predicate) {
    return RxObservable::wrap(op::Operators::some(
        predicate_cb<Variant>(predicate)
    )(this->_ptr));
}


/* BINDINGS GO HERE */
void RxObservable::_bind_methods() {
    RX_WRAPPER_CAST_BINDS(RxObservable)

    /* Constructors */
    ClassDB::bind_static_method("RxObservable", D_METHOD("case", "mapper", "sources", "default"), &RxObservable::case_mapper, DEFVAL(Ref<RxObservable>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable);

    ClassDB::bind_static_method("RxObservable", D_METHOD("combine_latest", "sources"), &RxObservable::combine_latest);

    ClassDB::bind_static_method("RxObservable", D_METHOD("concat", "sources"), &RxObservable::concat);
    
    ClassDB::bind_static_method("RxObservable", D_METHOD("defer", "factory"), &RxObservable::defer);

    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("fork_join", "sources"), &RxObservable::fork_join);

    ClassDB::bind_static_method("RxObservable", D_METHOD("from_iterable", "iterable", "scheduler"), &RxObservable::from_iterable, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("from", "iterable", "scheduler"), &RxObservable::from, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("generate", "initial_state", "condition", "iterate"), &RxObservable::generate);

    ClassDB::bind_static_method("RxObservable", D_METHOD("if_then", "condition", "then_source", "else_source"), &RxObservable::if_then, DEFVAL(Ref<RxObservable>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never);

    ClassDB::bind_static_method("RxObservable", D_METHOD("on_error_resume_next", "sources", "use_factory"), &RxObservable::on_error_resume_next, DEFVAL(false));

    ClassDB::bind_static_method("RxObservable", D_METHOD("range", "start", "stop", "step"), &RxObservable::range, DEFVAL(INT64_MAX), DEFVAL(1));

    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::just, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("raise", "message", "type", "scheduler"), &RxObservable::throw_message, DEFVAL("RxError"), DEFVAL(Ref<RxSchedulerBase>()));
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "error", "scheduler"), &RxObservable::throw_error, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("timer", "duetime", "period", "scheduler"), &RxObservable::timer, DEFVAL(Variant()), DEFVAL(Ref<RxSchedulerBase>()));
    ClassDB::bind_static_method("RxObservable", D_METHOD("periodic_timer", "period", "scheduler"), &RxObservable::periodic_timer, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("using", "resource_factory", "observable_factory"), &RxObservable::using_resource);

    ClassDB::bind_static_method("RxObservable", D_METHOD("with_latest_from", "parent", "sources"), &RxObservable::with_latest_from);

    ClassDB::bind_static_method("RxObservable", D_METHOD("zip", "sources"), &RxObservable::zip);

    /* OPERATORS */

    ClassDB::bind_method(D_METHOD("all", "predicate"), &RxObservable::all);

    ClassDB::bind_method(D_METHOD("amb", "right_source"), &RxObservable::amb);

    ClassDB::bind_method(D_METHOD("as_observable"), &RxObservable::as_observable);

    ClassDB::bind_method(D_METHOD("average", "key_mapper"), &RxObservable::average, DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("catch_with", "handler"), &RxObservable::catch_with_handler);

    ClassDB::bind_method(D_METHOD("combine_latest_withv", "sources"), &RxObservable::combine_latest_withv);
    BIND_VARARG_METHOD(RxObservable, combine_latest_with, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("concat_withv", "sources"), &RxObservable::concat_withv);
    BIND_VARARG_METHOD(RxObservable, concat_with, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("contains", "value", "comparer"), &RxObservable::contains, DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("count", "predicate"), &RxObservable::count, DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("debounce", "duetime", "scheduler"), &RxObservable::debounce, DEFVAL(Ref<RxSchedulerBase>()));
    ClassDB::bind_method(D_METHOD("throttle_with_mapper", "throttle_duration_mapper"), &RxObservable::throttle_with_mapper);

    ClassDB::bind_method(D_METHOD("default_if_empty", "default_value"), &RxObservable::default_if_empty, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("dematerialize"), &RxObservable::dematerialize);

    ClassDB::bind_method(D_METHOD("distinct", "key_mapper", "comparer"), &RxObservable::distinct, DEFVAL(Callable()), DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("distinct_until_changed", "key_mapper", "comparer"), &RxObservable::distinct_until_changed, DEFVAL(Callable()), DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("do_action", "on_next", "on_error", "on_completed"), &RxObservable::do_action, DEFVAL(Callable()), DEFVAL(Callable()), DEFVAL(Callable()));
    ClassDB::bind_method(D_METHOD("do", "observer"), &RxObservable::do_observer);
    ClassDB::bind_method(D_METHOD("do_after_next", "after_next"), &RxObservable::do_after_next);
    ClassDB::bind_method(D_METHOD("do_on_subscribe", "on_subscribe"), &RxObservable::do_on_subscribe);
    ClassDB::bind_method(D_METHOD("do_on_dispose", "on_dispose"), &RxObservable::do_on_dispose);
    ClassDB::bind_method(D_METHOD("do_on_terminate", "on_terminate"), &RxObservable::do_on_terminate);
    ClassDB::bind_method(D_METHOD("do_finally", "finally_action"), &RxObservable::do_finally);

    ClassDB::bind_method(D_METHOD("filter", "predicate"), &RxObservable::filter);
    ClassDB::bind_method(D_METHOD("filter_indexed", "predicate"), &RxObservable::filter_indexed, DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("last", "predicate"), &RxObservable::last, DEFVAL(Callable())); 

    ClassDB::bind_method(D_METHOD("last_or_default", "default_value", "predicate"), &RxObservable::last_or_default, DEFVAL(Variant()), DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("reduce", "accumulator", "seed"), &RxObservable::reduce, DEFVAL(Callable()), DEFVAL(memnew(NotSet)));

    ClassDB::bind_method(D_METHOD("map", "mapper"), &RxObservable::map, DEFVAL(Callable()));

    ClassDB::bind_method(D_METHOD("scan", "accumulator", "seed"), &RxObservable::scan, DEFVAL(memnew(NotSet)));

    ClassDB::bind_method(D_METHOD("some", "predicate"), &RxObservable::some, DEFVAL(Callable())); 

}

} // END namespace rx::wrappers