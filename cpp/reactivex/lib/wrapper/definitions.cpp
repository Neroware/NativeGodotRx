#include "wrapper/observable.h"

#include "observable/definitions.h"
#include "internal/utils.h"

using namespace rx::observable;

#define RX_ITERABLE_T(variant) rx::iterator::to_iterable(variant)
#define RX_PIPE(x) this->_ptr->pipe(x)
#define RX_VUNWRAP(argv) Array argv; for (auto argi = 0ul; argi < arg_count; argi++) argv.push_back(*(args[argi]));
#define RX_BIND_VARARG_METHOD(Cls, method_name, arg_type, arg_name) { \
        MethodInfo mi; \
		mi.arguments.push_back(PropertyInfo(Variant::arg_type, #arg_name)); \
		mi.name = #method_name; \
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, #method_name, &Cls::method_name, mi); \
    }

namespace rx::wrappers {

// case.h
Ref<RxObservable> RxObservable::case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source) { 
    return obs::case_(from_cb<Variant>(mapper), sources, default_source); 
}
// catch.h
Ref<RxObservable> RxObservable::catch_with_iterable(const Variant& sources) {
    return obs::catch_with_iterable_(RX_ITERABLE_T(sources)); 
}
// combinelatest.h
Ref<RxObservable> RxObservable::combine_latest(const Variant& sources) {
    return obs::combine_latest_(RX_ITERABLE_T(sources)); 
}
// concat.h
Ref<RxObservable> RxObservable::concat(const Variant& sources) {
    return obs::concat_with_iterable_(RX_ITERABLE_T(sources)); 
}
// defer.h
Ref<RxObservable> RxObservable::defer(const Callable& factory) {
    return obs::defer_(observable_factory_cb(factory)); 
}
// empty.h
Ref<RxObservable> RxObservable::empty(Ref<RxSchedulerBase> scheduler) { 
    return obs::empty_(scheduler); 
}
// forkjoin.h
Ref<RxObservable> RxObservable::fork_join(const Variant& sources) {
    return obs::fork_join_(RX_ITERABLE_T(sources)); 
}
// fromiterable.h
Ref<RxObservable> RxObservable::from_iterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return obs::from_iterable_(RX_ITERABLE_T(iterable), scheduler); 
}
Ref<RxObservable> RxObservable::from(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return obs::from_iterable_(RX_ITERABLE_T(iterable), scheduler); 
}
// generate.h
Ref<RxObservable> RxObservable::generate(const Variant& initial_state, const Callable& condition, const Callable& iterate) {
    return obs::generate_(initial_state, from_cb<bool, const Variant&>(condition), from_cb<Variant, const Variant&>(iterate)); 
}
// ifthen.h
Ref<RxObservable> RxObservable::if_then(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source) {
    return obs::if_then_(from_cb<bool>(condition), then_source, else_source);
}
// merge.h
Ref<RxObservable> RxObservable::merge(const Variant& sources) {
    return obs::merge_(RX_ITERABLE_T(sources)); 
}
// never.h
Ref<RxObservable> RxObservable::never() {
    return obs::never_();
}
// onerrorresumenext.h
Ref<RxObservable> RxObservable::on_error_resume_factory(const Variant& sources) {
    return obs::on_error_resume_next_(convert_all_cb<stated_observable_factory_t>(RX_ITERABLE_T(sources), stated_observable_factory_cb));
}
Ref<RxObservable> RxObservable::on_error_resume_next(const Variant& sources) {
    return obs::on_error_resume_next_(rx_observable_list_t(RX_ITERABLE_T(sources)));
}
// range.h
Ref<RxObservable> RxObservable::range(int64_t start, int64_t stop, int64_t step) {
    return obs::range_(start, stop, step);
}
// returnvalue.h
Ref<RxObservable> RxObservable::return_value(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return obs::return_value_(value, scheduler);
}
Ref<RxObservable> RxObservable::just(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return obs::return_value_(value, scheduler);
}
// throw.h
Ref<RxObservable> RxObservable::throw_message(const String& what, const String& type, Ref<RxSchedulerBase> scheduler) {
    try { throw rx_exception(what.ascii().get_data(), type.ascii().get_data()); } catch(...) { return obs::throw_(std::current_exception(), scheduler); }
}
Ref<RxObservable> RxObservable::throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler) {
    return obs::throw_(RxError::unwrap(error), scheduler); 
}
// timer.h
Ref<RxObservable> RxObservable::timer_abs(Ref<AbsoluteTime> duetime, Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return period.is_null() ? obs::timer_(duetime->t, scheduler) : obs::timer_(duetime->t, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::timer_rel(Ref<RelativeTime> duetime, Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return period.is_null() ? obs::timer_(duetime->dt, scheduler) : obs::timer_(duetime->dt, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::timer(double duetime, double period, Ref<RxSchedulerBase> scheduler) {
    return period < 0.0 ? obs::timer_(duetime, scheduler) : obs::timer_(duetime, Scheduler::to_timedelta(period), scheduler);
}
Ref<RxObservable> RxObservable::periodic_timer_rel(Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return obs::timer_(period->dt, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::periodic_timer(double period, Ref<RxSchedulerBase> scheduler) {
    return obs::timer_(period, Scheduler::to_timedelta(period), scheduler);
}
// using.h
Ref<RxObservable> RxObservable::using_resource(const Callable& resource_factory, const Callable& observable_factory) {
    return obs::using_(from_cb<disposable_t>(resource_factory), from_cb<rx_observable_t, const disposable_t&>(observable_factory));
}
// withlatestfrom.h
Ref<RxObservable> RxObservable::with_latest_from(Ref<RxObservable> parent, const Variant& sources) {
    return obs::with_latest_from_(parent, RX_ITERABLE_T(sources));
}
// zip.h
Ref<RxObservable> RxObservable::zip(const Variant& sources) {
    return obs::zip_(RX_ITERABLE_T(sources));
}


/* OPERATORS HERE */

// _all.h
Ref<RxObservable> RxObservable::all(const Callable& predicate) {
    return RX_PIPE(op::ops::all_(from_cb<bool, const Variant&>(predicate)));
}
// _amb.h
Ref<RxObservable> RxObservable::amb(Ref<RxObservable> right_source) {
    return RX_PIPE(op::ops::amb_(right_source));
}
// _asobservable.h
Ref<RxObservable> RxObservable::as_observable() {
    return RX_PIPE(op::ops::as_observable_());
}
// _average.h
Ref<RxObservable> RxObservable::average(const Callable& key_mapper) {
    return RX_PIPE(op::ops::average_(from_cb<double, const Variant&>(key_mapper)));
}
// _catch.h
Ref<RxObservable> RxObservable::catch_with_handler(const Callable& handler) {
    return RX_PIPE(op::ops::catch_(catch_handler_cb(handler)));
}
Ref<RxObservable> RxObservable::catch_with(Ref<RxObservable> obs) {
    return RX_PIPE(op::ops::catch_(obs));
}
// _combinelatest.h
Ref<RxObservable> RxObservable::combine_latest_withv(const Variant& others) {
    return RX_PIPE(op::ops::combine_latest_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::combine_latest_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::combine_latest_(RX_ITERABLE_T(others)));
}
// _concat.h.h
Ref<RxObservable> RxObservable::concat_withv(const Variant& others) {
    return RX_PIPE(op::ops::concat_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::concat_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::concat_(RX_ITERABLE_T(others)));
}
// _contains
Ref<RxObservable> RxObservable::contains(const Variant& value, const Callable& comparer) {
    return RX_PIPE(op::ops::contains_(value, comparer_cb(comparer)));
}
// _count.h
Ref<RxObservable> RxObservable::count(const Callable& predicate) {
    return RX_PIPE(op::ops::count_(predicate_cb<Variant>(predicate)));
}
// _debounce.h
Ref<RxObservable> RxObservable::debounce(const Variant& duetime, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::debounce_(get_dt(duetime), scheduler));
}
Ref<RxObservable> RxObservable::throttle_with_mapper(const Callable& throttle_duration_mapper) {
    return RX_PIPE(op::ops::throttle_with_mapper_(stated_observable_factory_cb(throttle_duration_mapper)));
}
// _defaultifempty.h
Ref<RxObservable> RxObservable::default_if_empty(const Variant& default_value) {
    return RX_PIPE(op::ops::default_if_empty_(default_value));
}
// _dematerialize.h
Ref<RxObservable> RxObservable::dematerialize() {
    return RX_PIPE(op::ops::dematerialize_());
}
// _distinct.h
Ref<RxObservable> RxObservable::distinct(const Callable& key_mapper, const Callable& comparer) {
    return RX_PIPE(op::ops::distinct_(from_cb<Variant, const Variant&>(key_mapper), comparer_cb(comparer)));
}
// _distinctuntilchanged.h
Ref<RxObservable> RxObservable::distinct_until_changed(const Callable& key_mapper, const Callable& comparer) {
    return RX_PIPE(op::ops::distinct_until_changed_(from_cb<Variant, const Variant&>(key_mapper), comparer_cb(comparer)));
}
// _do.h
Ref<RxObservable> RxObservable::do_action(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    return RX_PIPE(op::ops::do_action_(on_next_cb(on_next), on_error_cb(on_error), on_completed_cb(on_completed)));
}
Ref<RxObservable> RxObservable::do_observer(Ref<RxObserverBase> observer) {
    return RX_PIPE(op::ops::do_(observer));
}
Ref<RxObservable> RxObservable::do_after_next(const Callable& after_next) {
    return op::ops::do_after_next(this->_ptr, on_next_cb(after_next));
}
Ref<RxObservable> RxObservable::do_on_subscribe(const Callable& on_subscribe) {
    return op::ops::do_on_subscribe(this->_ptr, from_void_cb<>(on_subscribe));
}
Ref<RxObservable> RxObservable::do_on_dispose(const Callable& on_dispose) {
    return op::ops::do_on_dispose(this->_ptr, from_void_cb<>(on_dispose));
}
Ref<RxObservable> RxObservable::do_on_terminate(const Callable& on_terminate) {
    return op::ops::do_on_terminate(this->_ptr, from_void_cb<>(on_terminate));
}
Ref<RxObservable> RxObservable::do_finally(const Callable& finally_action) {
    return RX_PIPE(op::ops::do_finally_(from_void_cb<>(finally_action)));
}
// _elementatordefault.h
Ref<RxObservable> RxObservable::element_at_or_default(uint64_t index, bool has_default, const Variant& default_value) {
    return RX_PIPE(op::ops::element_at_or_default_(index, has_default, default_value));
}
// _exclusive.h
Ref<RxObservable> RxObservable::exclusive() {
    return RX_PIPE(op::ops::exclusive_());
}
// _expand.h
Ref<RxObservable> RxObservable::expand(const Callable& mapper) {
    return RX_PIPE(op::ops::expand_(from_cb<rx_observable_t, const Variant&>(mapper)));
}
// _finallyaction.h
Ref<RxObservable> RxObservable::finally_action(const Callable& action) {
    return RX_PIPE(op::ops::finally_action_(from_void_cb<>(action)));
}
// _find.h
Ref<RxObservable> RxObservable::find_value(const Callable& predicate, bool yield_index) {
    return RX_PIPE(op::ops::find_value_(predicate_cb<Variant, uint64_t, rx_observable_t>(predicate), yield_index));
}
// _first.h
Ref<RxObservable> RxObservable::first(const Callable& predicate) {
    return RX_PIPE(op::ops::first_(predicate_cb<Variant>(predicate)));
}
// _firstordefault
Ref<RxObservable> RxObservable::first_or_default(const Callable& predicate, const Variant& default_value) {
    return RX_PIPE(op::ops::first_or_default_(predicate_cb<Variant>(predicate), default_value));
}
// _forkjoin.h
Ref<RxObservable> RxObservable::fork_join_withv(const Variant& others) {
    return RX_PIPE(op::ops::fork_join_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::fork_join_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::fork_join_(RX_ITERABLE_T(others)));
}
// _filter.h
Ref<RxObservable> RxObservable::filter(const Callable& predicate) {
    return RX_PIPE(op::ops::filter_(predicate_cb<Variant>(predicate)));
}
Ref<RxObservable> RxObservable::filter_indexed(const Callable& predicate) {
    return RX_PIPE(op::ops::filter_indexed_(predicate_cb<Variant, uint64_t>(predicate)));
}
// _groupby.h
Ref<RxObservable> RxObservable::group_by(const Callable& key_mapper, const Callable& element_mapper, const Callable& subject_mapper) {
    return RX_PIPE(op::ops::group_by_(mapper_cb<Variant, Variant>(key_mapper), mapper_cb<Variant, Variant>(element_mapper), mapper_cb<rx_subject_t>(subject_mapper)));
}
// _groupbyuntil.h
Ref<RxObservable> RxObservable::group_by_until(const Callable& key_mapper, const Callable& duration_mapper, const Callable& element_mapper, const Callable& subject_mapper) {
    return RX_PIPE(op::ops::group_by_until_(mapper_cb<Variant, Variant>(key_mapper), mapper_cb<rx_observable_t, rx_grouped_observable_t>(duration_mapper), mapper_cb<Variant, Variant>(element_mapper), mapper_cb<rx_subject_t>(subject_mapper)));
}
// _groupjoin.h
Ref<RxObservable> RxObservable::group_join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper) {
    return RX_PIPE(op::ops::group_join_(right, mapper_cb<rx_observable_t, Variant>(left_duration_mapper), mapper_cb<rx_observable_t, Variant>(right_duration_mapper)));
}
// _ignoreelements.h
Ref<RxObservable> RxObservable::ignore_elements() {
    return RX_PIPE(op::ops::ignore_elements_());
}
// _isempty.h
Ref<RxObservable> RxObservable::is_empty() {
    return RX_PIPE(op::ops::is_empty_());
}
// _join.h
Ref<RxObservable> RxObservable::join(Ref<RxObservable> right, const Callable& left_duration_mapper, const Callable& right_duration_mapper) {
    return RX_PIPE(op::ops::join_(right, mapper_cb<rx_observable_t, Variant>(left_duration_mapper), mapper_cb<rx_observable_t, Variant>(right_duration_mapper)));
}
// _last.h
Ref<RxObservable> RxObservable::last(const Callable& predicate) {
    return RX_PIPE(op::ops::last_(predicate_cb<Variant>(predicate)));
}
// _lastordefault.h
Ref<RxObservable> RxObservable::last_or_default(const Variant& default_value, const Callable& predicate) {
    return RX_PIPE(op::ops::last_or_default_(default_value, predicate_cb<Variant>(predicate)));
}
// _map.h
Ref<RxObservable> RxObservable::map(const Callable& mapper) {
    return RX_PIPE(op::ops::map_(mapper_cb<Variant, Variant>(mapper)));
}
Ref<RxObservable> RxObservable::map_indexed(const Callable& mapper) {
    return RX_PIPE(op::ops::map_indexed_(mapper_cb<Variant, Variant, uint64_t>(mapper)));
}
// _materialize.h
Ref<RxObservable> RxObservable::materialize() {
    return RX_PIPE(op::ops::materialize_());
}
// _max.h
Ref<RxObservable> RxObservable::max(const Callable& comparer) {
    return RX_PIPE(op::ops::max_(comparer_cb(comparer)));
}
// _maxby.h
Ref<RxObservable> RxObservable::max_by(const Callable& key_mapper, const Callable& comparer) {
    return RX_PIPE(op::ops::max_by_(mapper_cb<Variant, Variant>(key_mapper), comparer_cb(comparer)));
}
// _merge.h
Ref<RxObservable> RxObservable::merge_all() {
    return RX_PIPE(op::ops::merge_all_());
}
Ref<RxObservable> RxObservable::merge_withv(const Variant& others, int64_t max_concurrent) {
    return RX_PIPE(op::ops::merge_(RX_ITERABLE_T(others), max_concurrent));
}
Ref<RxObservable> RxObservable::merge_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) int64_t max_concurrent = others.pop_front(); return RX_PIPE(op::ops::merge_(RX_ITERABLE_T(others), max_concurrent));
}
// _min.h
Ref<RxObservable> RxObservable::min(const Callable& comparer) {
    return RX_PIPE(op::ops::min_(comparer_cb(comparer)));
}
// _minby.h
Ref<RxObservable> RxObservable::min_by(const Callable& key_mapper, const Callable& comparer) {
    return RX_PIPE(op::ops::min_by_(mapper_cb<Variant, Variant>(key_mapper), comparer_cb(comparer)));
}
// _multicast
Ref<RxConnectableObservable> RxObservable::multicast(Ref<RxSubject> subject) {
    return rx_connectable_observable_t(RX_PIPE(op::ops::multicast_(subject)));
}
Ref<RxObservable> RxObservable::multicast_factory(const Callable& subject_factory, const Callable& mapper) {
    return RX_PIPE(op::ops::multicast_(rx_subject_factory_cb(subject_factory), mapper_cb<rx_observable_t, rx_observable_t>(mapper)));
}
// _observeon.h
Ref<RxObservable> RxObservable::observe_on(Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::observe_on_(scheduler));
}
// _onerrorresumenext.h
Ref<RxObservable> RxObservable::on_error_resume_next_with(Ref<RxObservable> second) {
    return RX_PIPE(op::ops::on_error_resume_next_(second));
}
// _pairwise.h
Ref<RxObservable> RxObservable::pairwise() {
    return RX_PIPE(op::ops::pairwise_());
}
// _partition.h
TypedArray<RxObservable> RxObservable::partition(const Callable& predicate) {
    TypedArray<RxObservable> res; auto partitions = RX_PIPE(op::ops::partition_(predicate_cb<Variant>(predicate))); for (const auto& partition : partitions) res.push_back(partition); return res;
}
TypedArray<RxObservable> RxObservable::partition_indexed(const Callable& predicate) {
    TypedArray<RxObservable> res; auto partitions = RX_PIPE(op::ops::partition_indexed_(predicate_cb<Variant, uint64_t>(predicate))); for (const auto& partition : partitions) res.push_back(partition); return res;
}
// _pluck.h
Ref<RxObservable> RxObservable::pluck(const Variant& key) {
    return RX_PIPE(op::ops::pluck_(key));
}
Ref<RxObservable> RxObservable::pluck_attr(const String& key) {
    return RX_PIPE(op::ops::pluck_attr_(key));
}
// _publish.h
Ref<RxConnectableObservable> RxObservable::publish() {
    return rx_connectable_observable_t(RX_PIPE(op::ops::publish_()));
}
Ref<RxObservable> RxObservable::publish_mapped(const Callable& mapper) {
    return RX_PIPE(op::ops::publish_(mapper_cb<rx_observable_t, rx_observable_t>(mapper)));
}
Ref<RxObservable> RxObservable::share() {
    return RX_PIPE(op::ops::share_());
}
// _publicvalue.h
Ref<RxObservable> RxObservable::publish_value(const Variant& initial_value, const Callable& mapper) {
    return RX_PIPE(op::ops::publish_value_(initial_value, mapper_cb<rx_observable_t, rx_observable_t>(mapper)));
}
// _reduce.h
Ref<RxObservable> RxObservable::reduce(const Callable& accumulator, const Variant& seed) {
    return RX_PIPE(op::ops::reduce_(accumulator_cb<Variant, Variant>(accumulator), seed));
}
// _scan.h
Ref<RxObservable> RxObservable::scan(const Callable& accumulator, const Variant& seed) {
    return RX_PIPE(op::ops::scan_(accumulator_cb<Variant, Variant>(accumulator), seed));
}
// _some.h
Ref<RxObservable> RxObservable::some(const Callable& predicate) {
    return RX_PIPE(op::ops::some_(predicate_cb<Variant>(predicate)));
}
// _take.h
Ref<RxObservable> RxObservable::take(uint64_t count) {
    return RX_PIPE(op::ops::take_(count));
}
// _timestamp.h
Ref<RxObservable> RxObservable::timestamp(Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::timestamp_(scheduler));
}
// _withlatestfrom.h
Ref<RxObservable> RxObservable::latest_from_withv(const Variant& sources) {
    return RX_PIPE(op::ops::with_latest_from_(RX_ITERABLE_T(sources)));
}
Ref<RxObservable> RxObservable::latest_from_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::with_latest_from_(RX_ITERABLE_T(others)));
}
// _zip.h
Ref<RxObservable> RxObservable::zip_withv(const Variant& others) {
    return RX_PIPE(op::ops::zip_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::zip_with(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::zip_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::zip_with_iterable(const Variant& iterable) {
    return RX_PIPE(op::ops::zip_with_iterable_(RX_ITERABLE_T(iterable)));
}

void RxObservable::_bind_methods() {
    /* Wrapper Casts */
    RX_WRAPPER_CAST_BINDS(RxObservable)

    /* Constructors */
    ClassDB::bind_static_method("RxObservable", D_METHOD("case", "mapper", "sources", "default"), &RxObservable::case_mapper, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable);

    ClassDB::bind_static_method("RxObservable", D_METHOD("combine_latest", "sources"), &RxObservable::combine_latest);

    ClassDB::bind_static_method("RxObservable", D_METHOD("concat", "sources"), &RxObservable::concat);
    
    ClassDB::bind_static_method("RxObservable", D_METHOD("defer", "factory"), &RxObservable::defer);

    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("fork_join", "sources"), &RxObservable::fork_join);

    ClassDB::bind_static_method("RxObservable", D_METHOD("from_iterable", "iterable", "scheduler"), &RxObservable::from_iterable, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("from", "iterable", "scheduler"), &RxObservable::from, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("generate", "initial_state", "condition", "iterate"), &RxObservable::generate);

    ClassDB::bind_static_method("RxObservable", D_METHOD("if_then", "condition", "then_source", "else_source"), &RxObservable::if_then, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("merge", "sources"), &RxObservable::merge);

    ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never);

    ClassDB::bind_static_method("RxObservable", D_METHOD("on_error_resume_next", "sources"), &RxObservable::on_error_resume_next);ClassDB::bind_static_method("RxObservable", D_METHOD("on_error_resume_factory", "sources"), &RxObservable::on_error_resume_factory);

    ClassDB::bind_static_method("RxObservable", D_METHOD("range", "start", "stop", "step"), &RxObservable::range, DEFVAL(INT64_MAX), DEFVAL(1));

    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::just, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("raise", "message", "type", "scheduler"), &RxObservable::throw_message, DEFVAL("RxError"), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "error", "scheduler"), &RxObservable::throw_error, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("timer_abs", "duetime", "period", "scheduler"), &RxObservable::timer_abs, DEFVAL(VNULL), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("timer_rel", "duetime", "period", "scheduler"), &RxObservable::timer_rel, DEFVAL(VNULL), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("timer", "duetime", "period", "scheduler"), &RxObservable::timer, DEFVAL(-1.0), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("periodic_timer", "period", "scheduler"), &RxObservable::periodic_timer, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("periodic_timer_rel", "period", "scheduler"), &RxObservable::periodic_timer_rel, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("using", "resource_factory", "observable_factory"), &RxObservable::using_resource);

    ClassDB::bind_static_method("RxObservable", D_METHOD("with_latest_from", "parent", "sources"), &RxObservable::with_latest_from);

    ClassDB::bind_static_method("RxObservable", D_METHOD("zip", "sources"), &RxObservable::zip);

    /* BIND OPERATORS HERE */

    ClassDB::bind_method(D_METHOD("all", "predicate"), &RxObservable::all);

    ClassDB::bind_method(D_METHOD("amb", "right_source"), &RxObservable::amb);

    ClassDB::bind_method(D_METHOD("as_observable"), &RxObservable::as_observable);

    ClassDB::bind_method(D_METHOD("average", "key_mapper"), &RxObservable::average, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("catch_with", "observable"), &RxObservable::catch_with);
    ClassDB::bind_method(D_METHOD("catch_with_handler", "handler"), &RxObservable::catch_with_handler);

    ClassDB::bind_method(D_METHOD("combine_latest_withv", "sources"), &RxObservable::combine_latest_withv);
    RX_BIND_VARARG_METHOD(RxObservable, combine_latest_with, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("concat_withv", "sources"), &RxObservable::concat_withv);
    RX_BIND_VARARG_METHOD(RxObservable, concat_with, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("contains", "value", "comparer"), &RxObservable::contains, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("count", "predicate"), &RxObservable::count, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("debounce", "duetime", "scheduler"), &RxObservable::debounce, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("throttle_with_mapper", "throttle_duration_mapper"), &RxObservable::throttle_with_mapper);

    ClassDB::bind_method(D_METHOD("default_if_empty", "default_value"), &RxObservable::default_if_empty, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("dematerialize"), &RxObservable::dematerialize);

    ClassDB::bind_method(D_METHOD("distinct", "key_mapper", "comparer"), &RxObservable::distinct, DEFVAL(CBNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("distinct_until_changed", "key_mapper", "comparer"), &RxObservable::distinct_until_changed, DEFVAL(CBNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("do_action", "on_next", "on_error", "on_completed"), &RxObservable::do_action, DEFVAL(CBNULL), DEFVAL(CBNULL), DEFVAL(CBNULL));
    ClassDB::bind_method(D_METHOD("do", "observer"), &RxObservable::do_observer);
    ClassDB::bind_method(D_METHOD("do_after_next", "after_next"), &RxObservable::do_after_next);
    ClassDB::bind_method(D_METHOD("do_on_subscribe", "on_subscribe"), &RxObservable::do_on_subscribe);
    ClassDB::bind_method(D_METHOD("do_on_dispose", "on_dispose"), &RxObservable::do_on_dispose);
    ClassDB::bind_method(D_METHOD("do_on_terminate", "on_terminate"), &RxObservable::do_on_terminate);
    ClassDB::bind_method(D_METHOD("do_finally", "finally_action"), &RxObservable::do_finally);

    ClassDB::bind_method(D_METHOD("element_at_or_default", "index", "has_default", "default_value"), &RxObservable::element_at_or_default, DEFVAL(false), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("exclusive"), &RxObservable::exclusive);

    ClassDB::bind_method(D_METHOD("expand", "mapper"), &RxObservable::expand);

    ClassDB::bind_method(D_METHOD("finally_action", "action"), &RxObservable::finally_action);

    ClassDB::bind_method(D_METHOD("find_value", "predicate", "yield_index"), &RxObservable::find_value);

    ClassDB::bind_method(D_METHOD("first", "predicate"), &RxObservable::first, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("first_or_default", "predicate"), &RxObservable::first_or_default, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("fork_join_withv", "sources"), &RxObservable::fork_join_withv);
    RX_BIND_VARARG_METHOD(RxObservable, fork_join_with, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("filter", "predicate"), &RxObservable::filter);
    ClassDB::bind_method(D_METHOD("filter_indexed", "predicate"), &RxObservable::filter_indexed, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("group_by", "key_mapper", "element_mapper", "subject_mapper"), &RxObservable::group_by, DEFVAL(CBNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("group_by_until", "key_mapper", "duration_mapper", "element_mapper", "subject_mapper"), &RxObservable::group_by_until, DEFVAL(CBNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("group_join", "right", "left_duration_mapper", "right_duration_mapper"), &RxObservable::group_join);

    ClassDB::bind_method(D_METHOD("ignore_elements"), &RxObservable::ignore_elements);

    ClassDB::bind_method(D_METHOD("is_empty"), &RxObservable::is_empty);

    ClassDB::bind_method(D_METHOD("join", "right", "left_duration_mapper", "right_duration_mapper"), &RxObservable::join);

    ClassDB::bind_method(D_METHOD("last", "predicate"), &RxObservable::last, DEFVAL(CBNULL)); 

    ClassDB::bind_method(D_METHOD("last_or_default", "default_value", "predicate"), &RxObservable::last_or_default, DEFVAL(VNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("map", "mapper"), &RxObservable::map, DEFVAL(Callable()));
    ClassDB::bind_method(D_METHOD("map_indexed", "mapper"), &RxObservable::map_indexed, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("materialize"), &RxObservable::materialize);

    ClassDB::bind_method(D_METHOD("max", "comparer"), &RxObservable::max, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("max_by", "key_mapper", "comparer"), &RxObservable::max_by, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("merge_all"), &RxObservable::merge_all);
    ClassDB::bind_method(D_METHOD("merge_withv", "sources", "max_concurrent"), &RxObservable::merge_withv, DEFVAL(-1));
    RX_BIND_VARARG_METHOD(RxObservable, merge_with, VARIANT_MAX, sources);

    ClassDB::bind_method(D_METHOD("min", "comparer"), &RxObservable::min, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("min_by", "key_mapper", "comparer"), &RxObservable::min_by, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("multicast", "subject"), &RxObservable::multicast);
    ClassDB::bind_method(D_METHOD("multicast_factory", "subject_factory", "mapper"), &RxObservable::multicast_factory, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("observe_on", "scheduler"), &RxObservable::observe_on);

    ClassDB::bind_method(D_METHOD("on_error_resume_next_with", "second"), &RxObservable::on_error_resume_next_with);

    ClassDB::bind_method(D_METHOD("pairwise"), &RxObservable::pairwise);

    ClassDB::bind_method(D_METHOD("partition", "predicate"), &RxObservable::partition);
    ClassDB::bind_method(D_METHOD("partition_indexed", "predicate"), &RxObservable::partition_indexed);

    ClassDB::bind_method(D_METHOD("pluck", "key"), &RxObservable::pluck);
    ClassDB::bind_method(D_METHOD("pluck_attr", "key"), &RxObservable::pluck_attr);

    ClassDB::bind_method(D_METHOD("publish"), &RxObservable::publish);
    ClassDB::bind_method(D_METHOD("publish_mapped", "mapper"), &RxObservable::publish_mapped);
    ClassDB::bind_method(D_METHOD("share"), &RxObservable::share);

    ClassDB::bind_method(D_METHOD("publish_value", "initial_value", "mapper"), &RxObservable::publish_value, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("reduce", "accumulator", "seed"), &RxObservable::reduce, DEFVAL(CBNULL), DEFVAL(memnew(NotSet)));

    ClassDB::bind_method(D_METHOD("scan", "accumulator", "seed"), &RxObservable::scan, DEFVAL(memnew(NotSet)));

    ClassDB::bind_method(D_METHOD("some", "predicate"), &RxObservable::some, DEFVAL(CBNULL)); 

    ClassDB::bind_method(D_METHOD("take", "count"), &RxObservable::take);

    ClassDB::bind_method(D_METHOD("timestamp", "scheduler"), &RxObservable::timestamp, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("latest_from_withv", "sources"), &RxObservable::latest_from_withv);
    RX_BIND_VARARG_METHOD(RxObservable, latest_from_with, OBJECT, "sources")

    ClassDB::bind_method(D_METHOD("zip_withv", "sources"), &RxObservable::zip_withv);
    RX_BIND_VARARG_METHOD(RxObservable, zip_with, OBJECT, "sources")
    ClassDB::bind_method(D_METHOD("zip_with_iterable", "iterable"), &RxObservable::zip_with_iterable);
}

} // END namespace rx::wrappers