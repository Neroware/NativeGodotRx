inline static Ref<RxObservable> empty(Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::empty_(RxSchedulerBase::unwrap(scheduler)));
}
inline static Ref<RxObservable> return_value(const Variant& value, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::return_value_(value, RxSchedulerBase::unwrap(scheduler)));
}
inline static Ref<RxObservable> never() {
    return RxObservable::wrap(Observables::never_());
}
inline static Ref<RxObservable> throw_message(const String& what, const String& type = "RxError", Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    try { throw rx_exception(what.ascii().get_data(), type.ascii().get_data()); } catch(...) {
        return RxObservable::wrap(Observables::throw_(std::current_exception(), RxSchedulerBase::unwrap(scheduler)));
    }
}
inline static Ref<RxObservable> throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::throw_(RxError::unwrap(error), RxSchedulerBase::unwrap(scheduler)));
}
inline static Ref<RxObservable> catch_with_iterable(const Variant& sources) {
    return RxObservable::wrap(Observables::catch_with_iterable_(
        rx_wrapper_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
    ));
}
inline static Ref<RxObservable> defer(const Callable& factory) {
    return RxObservable::wrap(Observables::defer_(observable_factory_cb(factory)));
}
inline static Ref<RxObservable> case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source = Ref<RxObservable>()) {
    return RxObservable::wrap(Observables::case_(
        std::function<Variant()>([mapper](){ return mapper.callv(Array()); }),
        dictionary_mapping<RxObservable, Observable>(sources),
        RxObservable::unwrap(default_source)
    ));
}
inline static Ref<RxObservable> combine_latest(const TypedArray<RxObservable>& sources) {
    return RxObservable::wrap(Observables::combine_latest_(array_container<RxObservable, Observable>(sources)));
}
inline static Ref<RxObservable> concat_with_iterable(const Variant& sources) {
    return RxObservable::wrap(Observables::concat_with_iterable_(
        rx_wrapper_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
    ));
}
inline static Ref<RxObservable> fork_join(const TypedArray<RxObservable>& sources) {
    return RxObservable::wrap(Observables::fork_join_(array_container<RxObservable, Observable>(sources)));
}
inline static Ref<RxObservable> from_iterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::from_iterable_(
        RxIterableBase::unwrap(rx::iterator::to_iterable(iterable)),
        RxSchedulerBase::unwrap(scheduler)
    ));
}
inline static Ref<RxObservable> generate(const Variant& initial_state, const Callable& condition, const Callable& iterate) {
    return RxObservable::wrap(Observables::generate_(
        initial_state,
        [condition](const Variant& state) -> bool { return condition.callv(Array::make(state)); },
        [iterate](const Variant& state) -> Variant { return iterate.callv(Array::make(state)); }
    ));
}
inline static Ref<RxObservable> if_then(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source) {
    return RxObservable::wrap(Observables::if_then_(
        [condition]() -> bool { return condition.callv(Array()); },
        RxObservable::unwrap(then_source),
        RxObservable::unwrap(else_source)
    ));
}
inline static Ref<RxObservable> on_error_resume_next(const Variant& sources, bool use_factory = false) {
    if (use_factory) {
        return RxObservable::wrap(Observables::on_error_resume_next_(
            rx_iterable<std::function<std::shared_ptr<Observable>(const std::exception_ptr&)>, Variant::Type::CALLABLE>(
                RxIterableBase::unwrap(rx::iterator::to_iterable(sources)),
                [](const Callable& cb) {
                    return [cb](const std::exception_ptr& e) -> std::shared_ptr<Observable> {
                        return RxObservable::unwrap(cb.callv(Array::make(RxError::wrap(e))));
                    };
                }
            )
        ));
    }
    return RxObservable::wrap(Observables::on_error_resume_next_(
        rx_wrapper_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
    ));
}
inline static Ref<RxObservable> range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1) {
    return RxObservable::wrap(Observables::range_(start, stop, step));
}
inline static Ref<RxObservable> timer(const Variant& duetime, const Variant& period = Variant(), Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
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
            RxObservable::wrap(Observables::periodic_timer_(_duetime->t, period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer_(_duetime->t, RxSchedulerBase::unwrap(scheduler)));
    }
    if (auto _duetime = DYN_CAST_OR_NULL(duetime, RelativeTime)) {
        return periodic ?
            RxObservable::wrap(Observables::periodic_timer_(_duetime->dt, period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer_(_duetime->dt, RxSchedulerBase::unwrap(scheduler)));
    }
    if (IS_NUMBER(duetime)) {
        double duetime_ = duetime;
        return periodic ?
            RxObservable::wrap(Observables::periodic_timer_(Scheduler::to_timedelta(duetime_), period_, RxSchedulerBase::unwrap(scheduler))) :
            RxObservable::wrap(Observables::timer_(Scheduler::to_timedelta(duetime_), RxSchedulerBase::unwrap(scheduler)));
    }
    throw BadArgumentException("No timespan type fits into timer observable constructor!");
}
inline static Ref<RxObservable> periodic_timer(const Variant& period, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return timer(period, period, scheduler);
}
inline static Ref<RxObservable> using_resource(const Callable& resource_factory, const Callable& observable_factory) {
    return RxObservable::wrap(Observables::using_(
        [resource_factory]() -> std::shared_ptr<DisposableBase> {
            return RxDisposableBase::unwrap(resource_factory.callv(Array())); 
        },
        [observable_factory](const std::shared_ptr<DisposableBase>& d) -> std::shared_ptr<Observable> { 
            return RxObservable::unwrap(observable_factory.callv(Array::make(RxDisposableBase::wrap(d)))); 
        }
    ));
}
inline static Ref<RxObservable> with_latest_from(Ref<RxObservable> parent, const TypedArray<RxObservable>& sources) {
    return RxObservable::wrap(Observables::with_latest_from_(RxObservable::unwrap(parent), array_container<RxObservable, Observable>(sources)));
}
inline static Ref<RxObservable> zip(const TypedArray<RxObservable>& sources) {
    return RxObservable::wrap(Observables::zip_(array_container<RxObservable, Observable>(sources)));
}


inline Ref<RxObservable> filter(const Callable& predicate) {
    return RxObservable::wrap(Operators::filter_(
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}
inline Ref<RxObservable> filter_indexed(const Callable& predicate) {
    return RxObservable::wrap(Operators::filter_indexed_(
        [predicate](const Variant& v, uint64_t i) -> bool { return predicate.callv(Array::make(v, i)); }
    )(this->_ptr));
}
inline Ref<RxObservable> amb(Ref<RxObservable> right_source) {
    return RxObservable::wrap(Operators::amb_(
        RxObservable::unwrap(right_source)
    )(this->_ptr));
}
inline Ref<RxObservable> as_observable() {
    return RxObservable::wrap(Operators::as_observable_()(this->_ptr));
}
inline Ref<RxObservable> map(const Callable& mapper) {
    return RxObservable::wrap(Operators::map_(
        [mapper](const Variant& v) -> Variant { return mapper.callv(Array::make(v)); }
    )(this->_ptr));
}
inline Ref<RxObservable> last_or_default(const Variant& default_value = Variant()) {
    return RxObservable::wrap(Operators::last_or_default_(default_value)(this->_ptr));
}
inline Ref<RxObservable> last_or_default_predicated(const Callable& predicate, const Variant& default_value = Variant()) {
    return RxObservable::wrap(Operators::last_or_default_(
        default_value,
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}
inline Ref<RxObservable> last() {
    return RxObservable::wrap(Operators::last_()(this->_ptr));
}
inline Ref<RxObservable> last_predicated(const Callable& predicate) {
    return RxObservable::wrap(Operators::last_(
        [predicate](const Variant& v) -> bool { return predicate.callv(Array::make(v)); }
    )(this->_ptr));
}