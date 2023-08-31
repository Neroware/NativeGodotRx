// case.h
static Ref<RxObservable> case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source = Ref<RxObservable>());
// catch.h
static Ref<RxObservable> catch_with_iterable(const Variant& sources);
// combinelatest.h
static Ref<RxObservable> combine_latest(const Variant& sources);
// concat.h
static Ref<RxObservable> concat(const Variant& sources);
// defer.h
static Ref<RxObservable> defer(const Callable& factory);
// empty.h
static Ref<RxObservable> empty(Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
// forkjoin.h
static Ref<RxObservable> fork_join(const Variant& sources);
// fromiterable.h
static Ref<RxObservable> from_iterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
static Ref<RxObservable> from(const Variant& iterable, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
// generate.h
static Ref<RxObservable> generate(const Variant& initial_state, const Callable& condition, const Callable& iterate);
// ifthen.h
static Ref<RxObservable> if_then(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source);
// never.h
static Ref<RxObservable> never();
// onerrorresumenext.h
static Ref<RxObservable> on_error_resume_next(const Variant& sources, bool use_factory = false);
// range.h
static Ref<RxObservable> range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1);
// returnvalue.h
static Ref<RxObservable> return_value(const Variant& value, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
static Ref<RxObservable> just(const Variant& value, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
// throw.h
static Ref<RxObservable> throw_message(const String& what, const String& type = "RxError", Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
static Ref<RxObservable> throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
// timer.h
static Ref<RxObservable> timer(const Variant& duetime, const Variant& period = Variant(), Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
static Ref<RxObservable> periodic_timer(const Variant& period, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
// using.h
static Ref<RxObservable> using_resource(const Callable& resource_factory, const Callable& observable_factory);
// withlatestfrom.h
static Ref<RxObservable> with_latest_from(Ref<RxObservable> parent, const Variant& sources);
// zip.h
static Ref<RxObservable> zip(const Variant& sources);


/* OPERATORS */

// _amb.h
Ref<RxObservable> amb(Ref<RxObservable> right_source);
// _asobservable.h
Ref<RxObservable> as_observable();
// _average.h
Ref<RxObservable> average(const Callable& key_mapper = Callable());
// _catch.h
Ref<RxObservable> catch_with_handler(const Variant& handler);
// _combine_latest.h
Ref<RxObservable> combine_latest_withv(const Variant& sources);
Ref<RxObservable> combine_latest_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _filter.h
Ref<RxObservable> filter(const Callable& predicate);
Ref<RxObservable> filter_indexed(const Callable& predicate = Callable());
// _last.h
Ref<RxObservable> last(const Callable& predicate = Callable());
// _lastordefault.h
Ref<RxObservable> last_or_default(const Variant& default_value = Variant(), const Callable& predicate = Callable());
// _map.h
Ref<RxObservable> map(const Callable& mapper = Callable());
// _scan.h
Ref<RxObservable> scan(const Callable& accumulator, const Variant& seed = NotSet::value());