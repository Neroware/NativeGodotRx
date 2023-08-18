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
        rx_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
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
        rx_iterable<RxObservable, Observable>(RxIterableBase::unwrap(rx::iterator::to_iterable(sources)))
    ));
}