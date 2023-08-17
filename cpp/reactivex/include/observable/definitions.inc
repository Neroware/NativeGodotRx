inline static Ref<RxObservable> empty(Ref<RxScheduler> scheduler = Ref<RxScheduler>()) {
    return RxObservable::wrap(Observables::empty_(RxScheduler::unwrap(scheduler)));
}
inline static Ref<RxObservable> return_value(const Variant& value, Ref<RxScheduler> scheduler = Ref<RxScheduler>()) {
    return RxObservable::wrap(Observables::return_value_(value, RxScheduler::unwrap(scheduler)));
}
inline static Ref<RxObservable> never() {
    return RxObservable::wrap(Observables::never_());
}
inline static Ref<RxObservable> throw_error(const String& what, Ref<RxScheduler> scheduler = Ref<RxScheduler>()) {
    try { throw rx_exception(what.ascii().get_data()); } catch(...) {
        return RxObservable::wrap(Observables::throw_(std::current_exception(), RxScheduler::unwrap(scheduler)));
    }
}
inline static Ref<RxObservable> catch_with_iterable(const Variant& sources) {
    return RxObservable::wrap(Observables::catch_with_iterable_(
        rx_iterable<RxObservable, ObservableBase>(RxIterable::unwrap(rx::iterator::to_iterable(sources)))
    ));
}
inline static Ref<RxObservable> defer(const Callable& factory) {
    return RxObservable::wrap(Observables::defer_(observable_factory_cb(factory)));
}