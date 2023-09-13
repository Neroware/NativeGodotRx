// case.h
static Ref<RxObservable> case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source = VNULL);
// catch.h
static Ref<RxObservable> catch_with_iterable(const Variant& sources);
// combinelatest.h
static Ref<RxObservable> combine_latest(const Variant& sources);
// concat.h
static Ref<RxObservable> concat(const Variant& sources);
// defer.h
static Ref<RxObservable> defer(const Callable& factory);
// empty.h
static Ref<RxObservable> empty(Ref<RxSchedulerBase> scheduler = VNULL);
// forkjoin.h
static Ref<RxObservable> fork_join(const Variant& sources);
// fromiterable.h
static Ref<RxObservable> from_iterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> from(const Variant& iterable, Ref<RxSchedulerBase> scheduler = VNULL);
// generate.h
static Ref<RxObservable> generate(const Variant& initial_state, const Callable& condition, const Callable& iterate);
// ifthen.h
static Ref<RxObservable> if_then(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source);
// merge.h
static Ref<RxObservable> merge(const Variant& sources);
// never.h
static Ref<RxObservable> never();
// onerrorresumenext.h
static Ref<RxObservable> on_error_resume_next(const Variant& sources);
static Ref<RxObservable> on_error_resume_factory(const Variant& sources);
// range.h
static Ref<RxObservable> range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1);
// returnvalue.h
static Ref<RxObservable> return_value(const Variant& value, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> just(const Variant& value, Ref<RxSchedulerBase> scheduler = VNULL);
// throw.h
static Ref<RxObservable> throw_message(const String& what, const String& type = "RxError", Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler = VNULL);
// timer.h
static Ref<RxObservable> timer_abs(Ref<AbsoluteTime> duetime, Ref<RelativeTime> period = VNULL, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> timer_rel(Ref<RelativeTime> duetime, Ref<RelativeTime> period = VNULL, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> timer(double duetime, double period = -1.0, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> periodic_timer_rel(Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler = VNULL);
static Ref<RxObservable> periodic_timer(double period, Ref<RxSchedulerBase> scheduler = VNULL);
// using.h
static Ref<RxObservable> using_resource(const Callable& resource_factory, const Callable& observable_factory);
// withlatestfrom.h
static Ref<RxObservable> with_latest_from(Ref<RxObservable> parent, const Variant& sources);
// zip.h
static Ref<RxObservable> zip(const Variant& sources);


/* OPERATORS */

// _all.h
Ref<RxObservable> all(const Callable& predicate);
// _amb.h
Ref<RxObservable> amb(Ref<RxObservable> right_source);
// _asobservable.h
Ref<RxObservable> as_observable();
// _average.h
Ref<RxObservable> average(const Callable& key_mapper = CBNULL);
// _catch.h
Ref<RxObservable> catch_with_handler(const Callable& handler);
Ref<RxObservable> catch_with(Ref<RxObservable> handler);
// _combine_latest.h
Ref<RxObservable> combine_latest_withv(const Variant& sources);
Ref<RxObservable> combine_latest_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _concat.h
Ref<RxObservable> concat_withv(const Variant& sources);
Ref<RxObservable> concat_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _contains.h
Ref<RxObservable> contains(const Variant& value, const Callable& comparer = CBNULL);
// _count.h
Ref<RxObservable> count(const Callable& predicate = CBNULL);
// _debounce.h
Ref<RxObservable> debounce(const Variant& duetime, Ref<RxSchedulerBase> scheduler = VNULL);
Ref<RxObservable> throttle_with_mapper(const Callable& throttle_duration_mapper);
// _defaultifempty.h
Ref<RxObservable> default_if_empty(const Variant& default_value = VNULL);
// _dematerialize.h
Ref<RxObservable> dematerialize();
// _distinct.h
Ref<RxObservable> distinct(const Callable& key_mapper = CBNULL, const Callable& comparer = CBNULL);
// _distinctuntilchanged.h
Ref<RxObservable> distinct_until_changed(const Callable& key_mapper = CBNULL, const Callable& comparer = CBNULL);
// _do.h
Ref<RxObservable> do_action(const Callable& on_next = CBNULL, const Callable& on_error = CBNULL, const Callable& on_completed = CBNULL);
Ref<RxObservable> do_observer(Ref<RxObserverBase> observer);
Ref<RxObservable> do_after_next(const Callable& after_next);
Ref<RxObservable> do_on_subscribe(const Callable& on_subscribe);
Ref<RxObservable> do_on_dispose(const Callable& on_dispose);
Ref<RxObservable> do_on_terminate(const Callable& on_terminate);
Ref<RxObservable> do_finally(const Callable& finally_action);
// _elementatordefault.h
Ref<RxObservable> element_at_or_default(uint64_t index, bool has_default = false, const Variant& default_value = VNULL);
// _exclusive.h
Ref<RxObservable> exclusive();
// _expand.h
Ref<RxObservable> expand(const Callable& mapper);
// _filter.h
Ref<RxObservable> filter(const Callable& predicate);
Ref<RxObservable> filter_indexed(const Callable& predicate = CBNULL);
// _finallyaction.h
Ref<RxObservable> finally_action(const Callable& action);
// _find.h
Ref<RxObservable> find_value(const Callable& predicate, bool yield_index);
// _first.h
Ref<RxObservable> first(const Callable& predicate = CBNULL);
// _firstordefault.h
Ref<RxObservable> first_or_default(const Callable& predicate = CBNULL, const Variant& default_value = VNULL);
// _forkjoin.h
Ref<RxObservable> fork_join_withv(const Variant& sources);
Ref<RxObservable> fork_join_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _groupby.h
Ref<RxObservable> group_by(const Callable& key_mapper, const Callable& element_mapper = CBNULL, const Callable& subject_mapper = CBNULL);
// _groupbyuntil.h
Ref<RxObservable> group_by_until(const Callable& key_mapper, const Callable& duration_mapper, const Callable& element_mapper = CBNULL, const Callable& subject_mapper = CBNULL);
// _groupjoin.h
Ref<RxObservable> group_join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _ignoreelements.h
Ref<RxObservable> ignore_elements();
// _isempty.h
Ref<RxObservable> is_empty();
// _join.h
Ref<RxObservable> join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _last.h
Ref<RxObservable> last(const Callable& predicate = CBNULL);
// _lastordefault.h
Ref<RxObservable> last_or_default(const Variant& default_value = VNULL, const Callable& predicate = CBNULL);
// _map.h
Ref<RxObservable> map(const Callable& mapper = CBNULL);
Ref<RxObservable> map_indexed(const Callable& mapper = CBNULL);
// _materialize.h
Ref<RxObservable> materialize();
// _max.h
Ref<RxObservable> max(const Callable& comparer = CBNULL);
// _maxby.h
Ref<RxObservable> max_by(const Callable& key_mapper, const Callable& comparer = CBNULL);
// _merge.h
Ref<RxObservable> merge_all();
Ref<RxObservable> merge_withv(const Variant& sources, int64_t max_concurrent = -1);
Ref<RxObservable> merge_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _min.h
Ref<RxObservable> min(const Callable& comparer = CBNULL);
// _minby.h
Ref<RxObservable> min_by(const Callable& key_mapper, const Callable& comparer = CBNULL);
// _multicast.h
Ref<RxConnectableObservable> multicast(Ref<RxSubject> subject);
Ref<RxObservable> multicast_factory(const Callable& subject_factory, const Callable& mapper = CBNULL);
// _observeon.h
Ref<RxObservable> observe_on(Ref<RxSchedulerBase> scheduler);
// _onerrorresumenext.h
Ref<RxObservable> on_error_resume_next_with(Ref<RxObservable> second);
// _pairwise.h
Ref<RxObservable> pairwise();
// _partition.h
TypedArray<RxObservable> partition(const Callable& predicate);
TypedArray<RxObservable> partition_indexed(const Callable& predicate);
// _pluck.h
Ref<RxObservable> pluck(const Variant& key);
Ref<RxObservable> pluck_attr(const String& key);
// _publish.h
Ref<RxConnectableObservable> publish();
Ref<RxObservable> publish_mapped(const Callable& mapper);
Ref<RxObservable> share();
// _publishvalue.h
Ref<RxObservable> publish_value(const Variant& initial_value, const Callable& mapper = CBNULL);
// _reduce.h
Ref<RxObservable> reduce(const Callable& accumulator, const Variant& seed = UNSET);
// _scan.h
Ref<RxObservable> scan(const Callable& accumulator, const Variant& seed = UNSET);
// _some.h
Ref<RxObservable> some(const Callable& predicate = CBNULL);
// _take.h
Ref<RxObservable> take(uint64_t count);
// _timestamp.h
Ref<RxObservable> timestamp(Ref<RxSchedulerBase> scheduler = VNULL);
// _withlatestfrom.h
Ref<RxObservable> latest_from_withv(const Variant& sources);
Ref<RxObservable> latest_from_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _zip.h
Ref<RxObservable> zip_withv(const Variant& sources);
Ref<RxObservable> zip_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
Ref<RxObservable> zip_with_iterable(const Variant& iterable);