inline static Ref<RxObservable> empty(Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::empty_(RxSchedulerBase::unwrap(scheduler)));
}
inline static Ref<RxObservable> return_value(const Variant& value, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    return RxObservable::wrap(Observables::return_value_(value, RxSchedulerBase::unwrap(scheduler)));
}
inline static Ref<RxObservable> never() {
    return RxObservable::wrap(Observables::never_());
}
inline static Ref<RxObservable> throw_error(const String& what, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
    try { throw rx_exception(what.ascii().get_data()); } catch(...) {
        return RxObservable::wrap(Observables::throw_(std::current_exception(), RxSchedulerBase::unwrap(scheduler)));
    }
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