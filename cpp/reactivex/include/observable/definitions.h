#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#define OBSERVABLE_CONSTRUCTORS \
    static std::shared_ptr<Observable> empty(const std::shared_ptr<SchedulerBase>& scheduler = nullptr); \
    static std::shared_ptr<Observable> return_value(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler_ = nullptr); \
    static std::shared_ptr<Observable> catch_with_iterable(const std::shared_ptr<IterableBase>& sources);

#define OBSERVABLE_CONSTRUCTORS_WRAPPERS \
    inline static Ref<RxObservable> empty(Ref<RxScheduler> scheduler = Ref<RxScheduler>()) { return RxObservable::wrap(Observable::empty(RxScheduler::unwrap(scheduler))); } \
    inline static Ref<RxObservable> return_value(const Variant& value, Ref<RxScheduler> scheduler = Ref<RxScheduler>()) { return RxObservable::wrap(Observable::return_value(value, RxScheduler::unwrap(scheduler))); } \
    inline static Ref<RxObservable> catch_with_iterable(const Variant& sources) { return RxObservable::wrap(Observable::catch_with_iterable(RxIterable::unwrap(GDRX->iter(sources)))); }

#define OBSERVABLE_CONSTRUCTORS_BINDS \
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable);

#endif // RX_OBSERVABLE_DEFINITIONS_H