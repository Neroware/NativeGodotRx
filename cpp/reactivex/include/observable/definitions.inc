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

// _all.h
Ref<RxObservable> all(const Callable& predicate);
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
// _concat.h
Ref<RxObservable> concat_withv(const Variant& sources);
Ref<RxObservable> concat_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _contains.h
Ref<RxObservable> contains(const Variant& value, const Callable& comparer = Callable());
// _count.h
Ref<RxObservable> count(const Callable& predicate = Callable());
// _debounce.h
Ref<RxObservable> debounce(const Variant& duetime, Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>());
Ref<RxObservable> throttle_with_mapper(const Callable& throttle_duration_mapper);
// _defaultifempty.h
Ref<RxObservable> default_if_empty(const Variant& default_value = VNULL);
// _dematerialize.h
Ref<RxObservable> dematerialize();
// _distinct.h
Ref<RxObservable> distinct(const Callable& key_mapper = Callable(), const Callable& comparer = Callable());
// _distinctuntilchanged.h
Ref<RxObservable> distinct_until_changed(const Callable& key_mapper = Callable(), const Callable& comparer = Callable());
// _do.h
Ref<RxObservable> do_action(const Callable& on_next = Callable(), const Callable& on_error = Callable(), const Callable& on_completed = Callable());
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
// _finallyaction.h
Ref<RxObservable> finally_action(const Callable& action);
// _find.h
Ref<RxObservable> find_value(const Callable& predicate, bool yield_index);
// _first.h
Ref<RxObservable> first(const Callable& predicate = Callable());
// _firstordefault.h
Ref<RxObservable> first_or_default(const Callable& predicate = Callable(), const Variant& default_value = VNULL);
// _forkjoin.h
Ref<RxObservable> fork_join_withv(const Variant& sources);
Ref<RxObservable> fork_join_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
// _filter.h
Ref<RxObservable> filter(const Callable& predicate);
Ref<RxObservable> filter_indexed(const Callable& predicate = Callable());
// _groupby.h
Ref<RxObservable> group_by(const Callable& key_mapper, const Callable& element_mapper = Callable(), const Callable& subject_mapper = Callable());
// _groupbyuntil.h
Ref<RxObservable> group_by_until(const Callable& key_mapper, const Callable& duration_mapper, const Callable& element_mapper = Callable(), const Callable& subject_mapper = Callable());
// _groupjoin.h
Ref<RxObservable> group_join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _ignoreelements.h
Ref<RxObservable> ignore_elements();
// _isempty.h
Ref<RxObservable> is_empty();
// _join.h
Ref<RxObservable> join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper);
// _last.h
Ref<RxObservable> last(const Callable& predicate = Callable());
// _lastordefault.h
Ref<RxObservable> last_or_default(const Variant& default_value = Variant(), const Callable& predicate = Callable());
// _reduce.h
Ref<RxObservable> reduce(const Callable& accumulator, const Variant& seed = NotSet::value());
// _map.h
Ref<RxObservable> map(const Callable& mapper = Callable());
Ref<RxObservable> map_indexed(const Callable& mapper = Callable());
// _materialize.h
Ref<RxObservable> materialize();
// _scan.h
Ref<RxObservable> scan(const Callable& accumulator, const Variant& seed = NotSet::value());
// _some.h
Ref<RxObservable> some(const Callable& predicate = Callable());
// _take.h
Ref<RxObservable> take(uint64_t count);
// _zip.h
Ref<RxObservable> zip_withv(const Variant& sources);
Ref<RxObservable> zip_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
Ref<RxObservable> zip_with_iterable(const Variant& iterable);