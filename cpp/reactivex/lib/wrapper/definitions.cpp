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

// amb.h
Ref<RxObservable> RxObservable::Amb(const Variant& sources) {
    return obs::amb_(RX_ITERABLE_T(sources));
}
// case.h
Ref<RxObservable> RxObservable::Case(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source) { 
    return obs::case_(from_cb<Variant>(mapper), sources, default_source); 
}
// catch.h
Ref<RxObservable> RxObservable::Catch(const Variant& sources) {
    return obs::catch_with_iterable_(RX_ITERABLE_T(sources)); 
}
// combinelatest.h
Ref<RxObservable> RxObservable::CombineLatest(const Variant& sources) {
    return obs::combine_latest_(RX_ITERABLE_T(sources)); 
}
// concat.h
Ref<RxObservable> RxObservable::Concat(const Variant& sources) {
    return obs::concat_with_iterable_(RX_ITERABLE_T(sources)); 
}
// create.h
Ref<RxObservable> RxObservable::Create(const Callable& subscribe) {
    return obs::create_(from_cb<disposable_t, const observer_t&, const scheduler_t&>(subscribe));
}
// defer.h
Ref<RxObservable> RxObservable::Defer(const Callable& factory) {
    return obs::defer_(observable_factory_cb(factory)); 
}
// empty.h
Ref<RxObservable> RxObservable::Empty(Ref<RxSchedulerBase> scheduler) { 
    return obs::empty_(scheduler); 
}
// forkjoin.h
Ref<RxObservable> RxObservable::ForkJoin(const Variant& sources) {
    return obs::fork_join_(RX_ITERABLE_T(sources)); 
}
// fromiterable.h
Ref<RxObservable> RxObservable::FromIterable(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return obs::from_iterable_(RX_ITERABLE_T(iterable), scheduler); 
}
Ref<RxObservable> RxObservable::From(const Variant& iterable, Ref<RxSchedulerBase> scheduler) {
    return obs::from_iterable_(RX_ITERABLE_T(iterable), scheduler); 
}
// generate.h
Ref<RxObservable> RxObservable::Generate(const Variant& initial_state, const Callable& condition, const Callable& iterate) {
    return obs::generate_(initial_state, from_cb<bool, const Variant&>(condition), from_cb<Variant, const Variant&>(iterate)); 
}
// generatewithrelativetime.h
Ref<RxObservable> RxObservable::GenerateWithRelativeTime(const Variant& initial_state, const Callable& condition, const Callable& iterate, const Callable& time_mapper) {
    return obs::generate_with_relative_time_(initial_state, predicate_cb<Variant>(condition), mapper_cb<Variant, Variant>(iterate), time_mapper_cb(time_mapper));
}
// ifthen.h
Ref<RxObservable> RxObservable::IfThen(const Callable& condition, Ref<RxObservable> then_source, Ref<RxObservable> else_source) {
    return obs::if_then_(from_cb<bool>(condition), then_source, else_source);
}
// interval.h
Ref<RxObservable> RxObservable::Interval(const Variant& period, Ref<RxSchedulerBase> scheduler) {
    return obs::interval_(get_dt(period), scheduler);
}
// merge.h
Ref<RxObservable> RxObservable::Merge(const Variant& sources) {
    return obs::merge_(RX_ITERABLE_T(sources)); 
}
// never.h
Ref<RxObservable> RxObservable::Never() {
    return obs::never_();
}
// onerrorresumenext.h
Ref<RxObservable> RxObservable::OnErrorResumeFactory(const Variant& sources) {
    return obs::on_error_resume_next_(convert_all_cb<stated_observable_factory_t>(RX_ITERABLE_T(sources), stated_observable_factory_cb));
}
Ref<RxObservable> RxObservable::OnErrorResumeNext(const Variant& sources) {
    return obs::on_error_resume_next_(rx_observable_list_t(RX_ITERABLE_T(sources)));
}
// range.h
Ref<RxObservable> RxObservable::Range(int64_t start, int64_t stop, int64_t step) {
    return obs::range_(start, stop, step);
}
// repeat.h
Ref<RxObservable> RxObservable::Repeat(const Variant& value, int64_t repeat_count) {
    return obs::repeat_(value, repeat_count);
}
// returnvalue.h
Ref<RxObservable> RxObservable::ReturnValue(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return obs::return_value_(value, scheduler);
}
Ref<RxObservable> RxObservable::Just(const Variant& value, Ref<RxSchedulerBase> scheduler) {
    return obs::return_value_(value, scheduler);
}
// throw.h
Ref<RxObservable> RxObservable::ThrowMessage(const String& what, const String& type, Ref<RxSchedulerBase> scheduler) {
    try { throw rx_exception(what.ascii().get_data(), type.ascii().get_data()); } catch(...) { return obs::throw_(std::current_exception(), scheduler); }
}
Ref<RxObservable> RxObservable::ThrowError(Ref<RxError> error, Ref<RxSchedulerBase> scheduler) {
    return obs::throw_(RxError::unwrap(error), scheduler); 
}
// timer.h
Ref<RxObservable> RxObservable::TimerAbs(Ref<AbsoluteTime> duetime, Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return period.is_null() ? obs::timer_(duetime->t, scheduler) : obs::timer_(duetime->t, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::TimerRel(Ref<RelativeTime> duetime, Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return period.is_null() ? obs::timer_(duetime->dt, scheduler) : obs::timer_(duetime->dt, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::Timer(double duetime, double period, Ref<RxSchedulerBase> scheduler) {
    return period < 0.0 ? obs::timer_(duetime, scheduler) : obs::timer_(duetime, Scheduler::to_timedelta(period), scheduler);
}
Ref<RxObservable> RxObservable::PeriodicTimerRel(Ref<RelativeTime> period, Ref<RxSchedulerBase> scheduler) {
    return obs::timer_(period->dt, period->dt, scheduler);
}
Ref<RxObservable> RxObservable::PeriodicTimer(double period, Ref<RxSchedulerBase> scheduler) {
    return obs::timer_(period, Scheduler::to_timedelta(period), scheduler);
}
// using.h
Ref<RxObservable> RxObservable::Using(const Callable& resource_factory, const Callable& observable_factory) {
    return obs::using_(from_cb<disposable_t>(resource_factory), from_cb<rx_observable_t, const disposable_t&>(observable_factory));
}
// withlatestfrom.h
Ref<RxObservable> RxObservable::WithLatestFrom(Ref<RxObservable> parent, const Variant& sources) {
    return obs::with_latest_from_(parent, RX_ITERABLE_T(sources));
}
// zip.h
Ref<RxObservable> RxObservable::Zip(const Variant& sources) {
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
// _buffer.h
Ref<RxObservable> RxObservable::buffer(Ref<RxObservable> boundaries) {
    return RX_PIPE(op::ops::buffer_(boundaries));
}
Ref<RxObservable> RxObservable::buffer_when(const Callable& closing_mapper) {
    return RX_PIPE(op::ops::buffer_when_(mapper_cb<rx_observable_t>(closing_mapper)));
}
Ref<RxObservable> RxObservable::buffer_toggle(Ref<RxObservable> openings, const Callable& closing_mapper) {
    return RX_PIPE(op::ops::buffer_toggle_(openings, mapper_cb<rx_observable_t, Variant>(closing_mapper)));
}
Ref<RxObservable> RxObservable::buffer_with_count(int64_t count, int64_t skip) {
    return RX_PIPE(op::ops::buffer_with_count_(count, skip));
}
// _bufferwithtime.h
Ref<RxObservable> RxObservable::buffer_with_time(const Variant& timespan, const Variant& timeshift, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::buffer_with_time_(get_dt(timespan), get_dt(timeshift), scheduler));
}
// _bufferwithtimeorcount.h
Ref<RxObservable> RxObservable::buffer_with_time_or_count(const Variant& timespan, int64_t count, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::buffer_with_time_or_count_(get_dt(timespan), count, scheduler));
}
// _catch.h
Ref<RxObservable> RxObservable::catch_with_handler(const Callable& handler) {
    return RX_PIPE(op::ops::catch_(catch_handler_cb(handler)));
}
Ref<RxObservable> RxObservable::catch_with(Ref<RxObservable> obs) {
    return RX_PIPE(op::ops::catch_(obs));
}
// _combinelatest.h
Ref<RxObservable> RxObservable::combine_latestv(const Variant& others) {
    return RX_PIPE(op::ops::combine_latest_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::combine_latest(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::combine_latest_(RX_ITERABLE_T(others)));
}
// _concat.h.h
Ref<RxObservable> RxObservable::concatv(const Variant& others) {
    return RX_PIPE(op::ops::concat_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::concat(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
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
// _delay.h
Ref<RxObservable> RxObservable::delay(const Variant& duetime, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::delay_(get_dt(duetime), scheduler));
}
// _delaysubscription.h
Ref<RxObservable> RxObservable::delay_subscription(const Variant& duetime, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::delay_subscription_(get_dt(duetime), scheduler));
}
// _delaywithmapper.h
Ref<RxObservable> RxObservable::delay_with_mapper(Ref<RxObservable> subscription_delay, const Callable& subscription_delay_mapper, Ref<RxObservable> delay_duration_mapper) {
    return RX_PIPE(op::ops::delay_with_mapper_(subscription_delay, mapper_cb<Variant, Variant>(subscription_delay_mapper), delay_duration_mapper));
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
// _whiledo.h
Ref<RxObservable> RxObservable::do_while(const Callable& condition) {
    return RX_PIPE(op::ops::do_while_(predicate_cb<rx_observable_t>(condition)));
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
// _flatmap.h
Ref<RxObservable> RxObservable::flat_map(const Callable& mapper) {
    return RX_PIPE(op::ops::flat_map_(mapper_cb<rx_observable_t, Variant>(mapper)));
}
Ref<RxObservable> RxObservable::flat_map_indexed(const Callable& mapper) {
    return RX_PIPE(op::ops::flat_map_indexed_(mapper_cb<rx_observable_t, Variant, uint64_t>(mapper)));
}
Ref<RxObservable> RxObservable::flat_map_latest(const Callable& mapper) {
    return RX_PIPE(op::ops::flat_map_latest_(mapper_cb<rx_observable_t, Variant>(mapper)));
}
// _forkjoin.h
Ref<RxObservable> RxObservable::fork_joinv(const Variant& others) {
    return RX_PIPE(op::ops::fork_join_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::fork_join(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
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
Ref<RxObservable> RxObservable::mergev(const Variant& others, int64_t max_concurrent) {
    return RX_PIPE(op::ops::merge_(RX_ITERABLE_T(others), max_concurrent));
}
Ref<RxObservable> RxObservable::merge(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
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
Ref<RxObservable> RxObservable::on_error_resume_next(Ref<RxObservable> second) {
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
// _repeat.h
Ref<RxObservable> RxObservable::repeat(int64_t repeat_count) {
    return RX_PIPE(op::ops::repeat_(repeat_count));
}
// _replay.h
Ref<RxObservable> RxObservable::replay(const Callable& mapper, uint64_t buffer_size, const Variant& window, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::replay_(mapper_cb<rx_observable_t, rx_observable_t>(mapper), buffer_size, get_dt(window), scheduler));
}
// _retry.h
Ref<RxObservable> RxObservable::retry(int64_t retry_count) {
    return RX_PIPE(op::ops::retry_(retry_count));
}
// _sample.h
Ref<RxObservable> RxObservable::sample_with_time(const Variant& sampler, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::sample_(get_dt(sampler), scheduler));
}
Ref<RxObservable> RxObservable::sample(Ref<RxObservable> sampler) {
    return RX_PIPE(op::ops::sample_(sampler));
}
// _scan.h
Ref<RxObservable> RxObservable::scan(const Callable& accumulator, const Variant& seed) {
    return RX_PIPE(op::ops::scan_(accumulator_cb<Variant, Variant>(accumulator), seed));
}
// _sequenceequal.h
Ref<RxObservable> RxObservable::sequence_equal(Ref<RxObservable> second, const Callable& comparer) {
    return RX_PIPE(op::ops::sequence_equal_(second, comparer_cb(comparer)));
}
Ref<RxObservable>  RxObservable::sequence_equal_iterable(const Variant& iterable, const Callable& comparer) {
    return RX_PIPE(op::ops::sequence_equal_(RX_ITERABLE_T(iterable), comparer_cb(comparer)));
}
// _single.h
Ref<RxObservable> RxObservable::single(const Callable& predicate) {
    return RX_PIPE(op::ops::single_(predicate_cb<Variant>(predicate)));
}
// _singleordefault.h
Ref<RxObservable> RxObservable::single_or_default_async(bool has_default, const Variant& default_value) {
    return RX_PIPE(op::ops::single_or_default_async_(has_default, default_value));
}
Ref<RxObservable> RxObservable::single_or_default(const Callable& predicate, const Variant& default_value) {
    return RX_PIPE(op::ops::single_or_default_(predicate_cb<Variant>(predicate), default_value));
}
// _skip.h
Ref<RxObservable> RxObservable::skip(uint64_t count) {
    return RX_PIPE(op::ops::skip_(count));
}
// _skiplast.h
Ref<RxObservable> RxObservable::skip_last(uint64_t count) {
    return RX_PIPE(op::ops::skip_last_(count));
}
// _skiplastwithtime.h
Ref<RxObservable> RxObservable::skip_last_with_time_rel(Ref<RelativeTime> duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_last_with_time_(duration->dt, scheduler));
}
Ref<RxObservable> RxObservable::skip_last_with_time_abs(Ref<AbsoluteTime> duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_last_with_time_(duration->t, scheduler));
}
Ref<RxObservable> RxObservable::skip_last_with_time(double duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_last_with_time_(get_dt(duration), scheduler));
}
// _skipuntil.h
Ref<RxObservable> RxObservable::skip_until(Ref<RxObservable> other) {
    return RX_PIPE(op::ops::skip_until_(other));
}
// _skipuntilwithtime.h
Ref<RxObservable> RxObservable::skip_until_with_time_rel(Ref<RelativeTime> start_time, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_until_with_time_(start_time->dt, scheduler));
}
Ref<RxObservable> RxObservable::skip_until_with_time_abs(Ref<AbsoluteTime> start_time, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_until_with_time_(start_time->t, scheduler));
}
Ref<RxObservable> RxObservable::skip_until_with_time(double start_time, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_until_with_time_(get_dt(start_time), scheduler));
}
// _skipwhile.h
Ref<RxObservable> RxObservable::skip_while(const Callable& predicate) {
    return RX_PIPE(op::ops::skip_while_(predicate_cb<Variant>(predicate)));
}
Ref<RxObservable> RxObservable::skip_while_indexed(const Callable& predicate) {
    return RX_PIPE(op::ops::skip_while_indexed_(predicate_cb<Variant, uint64_t>(predicate)));
}
// _skipwithtime.h
Ref<RxObservable> RxObservable::skip_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::skip_with_time_(get_dt(duration), scheduler));
}
// _slice.h
Ref<RxObservable> RxObservable::slice(int64_t start, int64_t stop, int64_t step) {
    return RX_PIPE(op::ops::slice_(start, stop, step));
}
// _some.h
Ref<RxObservable> RxObservable::some(const Callable& predicate) {
    return RX_PIPE(op::ops::some_(predicate_cb<Variant>(predicate)));
}
// _startwith.h
Ref<RxObservable> RxObservable::start_with(const Variant& args) {
    return RX_PIPE(op::ops::start_with_(RxList<Variant>(RX_ITERABLE_T(args))));
}
// _subscribeon.h
Ref<RxObservable> RxObservable::subscribe_on(Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::subscribe_on_(scheduler));
}
// _sum.h
Ref<RxObservable> RxObservable::sum(const Callable& key_mapper) {
    return RX_PIPE(op::ops::sum_(mapper_cb<Variant, Variant>(key_mapper)));
}
// _switchlatest.h
Ref<RxObservable> RxObservable::switch_latest() {
    return RX_PIPE(op::ops::switch_latest_());
}
// _take.h
Ref<RxObservable> RxObservable::take(uint64_t count) {
    return RX_PIPE(op::ops::take_(count));
}
// _takelast.h
Ref<RxObservable> RxObservable::take_last(uint64_t count) {
    return RX_PIPE(op::ops::take_last_(count));
}
// _takelastwithbuffer.h
Ref<RxObservable> RxObservable::take_last_buffer(uint64_t count) {
    return RX_PIPE(op::ops::take_last_buffer_(count));
}
// _takelastwithtime.h
Ref<RxObservable> RxObservable::take_last_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::take_last_with_time_(get_dt(duration), scheduler));
}
// _takeuntil.h
Ref<RxObservable> RxObservable::take_until(Ref<RxObservable> other) {
    return RX_PIPE(op::ops::take_until_(other));
}
// _takeuntilwithtime.h
Ref<RxObservable> RxObservable::take_until_with_time(const Variant& end_time, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::take_until_with_time_(get_dt(end_time), scheduler));
}
// _takewhile.h
Ref<RxObservable> RxObservable::take_while(const Callable& predicate, bool inclusive) {
    return RX_PIPE(op::ops::take_while_(predicate_cb<Variant>(predicate), inclusive));
}
Ref<RxObservable> RxObservable::take_while_indexed(const Callable& predciate, bool inclusive) {
    return RX_PIPE(op::ops::take_while_indexed_(predicate_cb<Variant, uint64_t>(predciate), inclusive));
}
// _takewithtime.h
Ref<RxObservable> RxObservable::take_with_time(const Variant& duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::take_with_time_(get_dt(duration), scheduler));
}
// _throttlefirst.h
Ref<RxObservable> RxObservable::throttle_first(const Variant& window_duration, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::throttle_first_(get_dt(window_duration), scheduler));
}
// _timeinterval.h
Ref<RxObservable> RxObservable::time_interval(Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::time_interval_(scheduler));
}
// _timeout.h
Ref<RxObservable> RxObservable::timeout(const Variant& duetime, Ref<RxObservable> other, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::timeout_(get_dt(duetime), other, scheduler));
}
// _timeoutwithmapper.h
Ref<RxObservable> RxObservable::timeout_with_mapper(Ref<RxObservable> first_timeout, const Callable& timeout_duration_mapper, Ref<RxObservable> other) {
    return RX_PIPE(op::ops::timeout_with_mapper_(first_timeout, mapper_cb<rx_observable_t, Variant>(timeout_duration_mapper), other));
}
// _timestamp.h
Ref<RxObservable> RxObservable::timestamp(Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::timestamp_(scheduler));
}
// _todict.h
Ref<RxObservable> RxObservable::to_dict(const Callable& key_mapper, const Callable& element_mapper) {
    return RX_PIPE(op::ops::to_dict_(mapper_cb<Variant, Variant>(key_mapper), mapper_cb<Variant, Variant>(key_mapper)));
}
// _tolist.h
Ref<RxObservable> RxObservable::to_list() {
    return RX_PIPE(op::ops::to_list_());
}
Ref<RxObservable> RxObservable::to_iterable() {
    return RX_PIPE(op::ops::to_iterable_());
}
// _toset.h
Ref<RxObservable> RxObservable::to_set(const Callable& comparer) {
    return RX_PIPE(op::ops::to_set_(comparer_cb(comparer)));
}
// _whiledo.h
Ref<RxObservable> RxObservable::while_do(const Callable& condition) {
    return RX_PIPE(op::ops::while_do_(predicate_cb<rx_observable_t>(condition)));
}
// _window.h
Ref<RxObservable> RxObservable::window_toggle(Ref<RxObservable> openings, const Callable& closing_mapper) {
    return RX_PIPE(op::ops::window_toggle_(openings, mapper_cb<rx_observable_t, Variant>(closing_mapper)));
}
Ref<RxObservable> RxObservable::window(Ref<RxObservable> boundaries) {
    return RX_PIPE(op::ops::window_(boundaries));
}
Ref<RxObservable> RxObservable::window_when(const Callable& closing_mapper) {
    return RX_PIPE(op::ops::window_when_(mapper_cb<rx_observable_t>(closing_mapper)));
}
// _windowwithcount.h
Ref<RxObservable> RxObservable::window_with_count(int64_t count, int64_t skip) {
    return RX_PIPE(op::ops::window_with_count_(count, skip));
}
// _windowwithtime.h
Ref<RxObservable> RxObservable::window_with_time(const Variant& timespan, const Variant& timeshift, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::window_with_time_(get_dt(timespan), get_dt(timeshift), scheduler));
}
// _windowwithtimeorcount.h
Ref<RxObservable> RxObservable::window_with_time_or_count(const Variant& timespan, int64_t count, Ref<RxSchedulerBase> scheduler) {
    return RX_PIPE(op::ops::window_with_time_or_count_(get_dt(timespan), count, scheduler));
}
// _withlatestfrom.h
Ref<RxObservable> RxObservable::with_latest_fromv(const Variant& sources) {
    return RX_PIPE(op::ops::with_latest_from_(RX_ITERABLE_T(sources)));
}
Ref<RxObservable> RxObservable::with_latest_from(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::with_latest_from_(RX_ITERABLE_T(others)));
}
// _zip.h
Ref<RxObservable> RxObservable::zipv(const Variant& others) {
    return RX_PIPE(op::ops::zip_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::zip(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    RX_VUNWRAP(others) return RX_PIPE(op::ops::zip_(RX_ITERABLE_T(others)));
}
Ref<RxObservable> RxObservable::zip_with_iterable(const Variant& iterable) {
    return RX_PIPE(op::ops::zip_with_iterable_(RX_ITERABLE_T(iterable)));
}

// _refcount.h
Ref<RxObservable> RxObservable::add_ref(Ref<RxObservable> xs, Ref<RxRefCountDisposable> r) {
    return connectable::add_ref(xs, r);
}
Ref<RxObservable> RxObservable::ref_count() {
    return RX_PIPE(connectable::ref_count_());
}

// _fromsignal.h

Ref<RxObservable> RxObservable::from_signal(Object* owner, const StringName& signal_name, bool track_owner, Ref<RxSchedulerBase> scheduler) {
    return rx::observable::godot::from_signal_(owner, signal_name, track_owner, scheduler);
}

void RxObservable::_bind_methods() {
    /* Wrapper Casts */
    RX_WRAPPER_CAST_BINDS(RxObservable)

    /* Constructors */
    ClassDB::bind_static_method("RxObservable", D_METHOD("Amb", "sources"), &RxObservable::Amb);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Case", "mapper", "sources", "default"), &RxObservable::Case, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Catch", "sources"), &RxObservable::Catch);

    ClassDB::bind_static_method("RxObservable", D_METHOD("CombineLatest", "sources"), &RxObservable::CombineLatest);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Concat", "sources"), &RxObservable::Concat);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Create", "subscribe"), &RxObservable::Create, DEFVAL(CBNULL));
    
    ClassDB::bind_static_method("RxObservable", D_METHOD("Defer", "factory"), &RxObservable::Defer);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Empty", "scheduler"), &RxObservable::Empty, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("ForkJoin", "sources"), &RxObservable::ForkJoin);

    ClassDB::bind_static_method("RxObservable", D_METHOD("FromIterable", "iterable", "scheduler"), &RxObservable::FromIterable, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("From", "iterable", "scheduler"), &RxObservable::From, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Generate", "initial_state", "condition", "iterate"), &RxObservable::Generate);

    ClassDB::bind_static_method("RxObservable", D_METHOD("GenerateWithRelativeTime", "initial_state", "condition", "iterate", "time_mapper"), &RxObservable::GenerateWithRelativeTime);

    ClassDB::bind_static_method("RxObservable", D_METHOD("IfThen", "condition", "then_source", "else_source"), &RxObservable::IfThen, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Interval", "period", "scheduler"), &RxObservable::Interval, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Merge", "sources"), &RxObservable::Merge);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Never"), &RxObservable::Never);

    ClassDB::bind_static_method("RxObservable", D_METHOD("OnErrorResumeNext", "sources"), &RxObservable::OnErrorResumeNext);
    ClassDB::bind_static_method("RxObservable", D_METHOD("OnErrorResumeFactory", "sources"), &RxObservable::OnErrorResumeFactory);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Range", "start", "stop", "step"), &RxObservable::Range, DEFVAL(INT64_MAX), DEFVAL(1));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Repeat", "value", "repeat_count"), &RxObservable::Repeat, DEFVAL(-1));

    ClassDB::bind_static_method("RxObservable", D_METHOD("ReturnValue", "value", "scheduler"), &RxObservable::ReturnValue, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("Just", "value", "scheduler"), &RxObservable::Just, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Raise", "message", "type", "scheduler"), &RxObservable::ThrowMessage, DEFVAL("RxError"), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("Throw", "error", "scheduler"), &RxObservable::ThrowError, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("TimerAbs", "duetime", "period", "scheduler"), &RxObservable::TimerAbs, DEFVAL(VNULL), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("TimerRel", "duetime", "period", "scheduler"), &RxObservable::TimerRel, DEFVAL(VNULL), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("Timer", "duetime", "period", "scheduler"), &RxObservable::Timer, DEFVAL(-1.0), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("PeriodicTimer", "period", "scheduler"), &RxObservable::PeriodicTimer, DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("PeriodicTimerRel", "period", "scheduler"), &RxObservable::PeriodicTimerRel, DEFVAL(VNULL));

    ClassDB::bind_static_method("RxObservable", D_METHOD("Using", "resource_factory", "observable_factory"), &RxObservable::Using);

    ClassDB::bind_static_method("RxObservable", D_METHOD("WithLatestFrom", "parent", "sources"), &RxObservable::WithLatestFrom);

    ClassDB::bind_static_method("RxObservable", D_METHOD("Zip", "sources"), &RxObservable::Zip);

    /* BIND OPERATORS HERE */

    ClassDB::bind_method(D_METHOD("all", "predicate"), &RxObservable::all);

    ClassDB::bind_method(D_METHOD("amb", "right_source"), &RxObservable::amb);

    ClassDB::bind_method(D_METHOD("as_observable"), &RxObservable::as_observable);

    ClassDB::bind_method(D_METHOD("average", "key_mapper"), &RxObservable::average, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("buffer", "boundaries"), &RxObservable::buffer);
    ClassDB::bind_method(D_METHOD("buffer_when", "closing_mapper"), &RxObservable::buffer_when);
    ClassDB::bind_method(D_METHOD("buffer_toggle", "openings", "closing_mapper"), &RxObservable::buffer_toggle);
    ClassDB::bind_method(D_METHOD("buffer_with_count", "count", "skip"), &RxObservable::buffer_with_count, DEFVAL(-1));

    ClassDB::bind_method(D_METHOD("buffer_with_time", "timespan", "timeshift", "scheduler"), &RxObservable::buffer_with_time, DEFVAL(DTZERO), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("buffer_with_time_or_count", "timespan", "count", "scheduler"), &RxObservable::buffer_with_time_or_count, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("catch", "observable"), &RxObservable::catch_with);
    ClassDB::bind_method(D_METHOD("catch_with_handler", "handler"), &RxObservable::catch_with_handler);

    ClassDB::bind_method(D_METHOD("combine_latestv", "sources"), &RxObservable::combine_latestv);
    RX_BIND_VARARG_METHOD(RxObservable, combine_latest, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("concatv", "sources"), &RxObservable::concatv);
    RX_BIND_VARARG_METHOD(RxObservable, concat, OBJECT, sources)

    ClassDB::bind_method(D_METHOD("contains", "value", "comparer"), &RxObservable::contains, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("count", "predicate"), &RxObservable::count, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("debounce", "duetime", "scheduler"), &RxObservable::debounce, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("throttle_with_mapper", "throttle_duration_mapper"), &RxObservable::throttle_with_mapper);

    ClassDB::bind_method(D_METHOD("default_if_empty", "default_value"), &RxObservable::default_if_empty, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("delay", "duetime", "scheduler"), &RxObservable::delay, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("delay_subscription", "duetime", "scheduler"), &RxObservable::delay_subscription, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("delay_with_mapper", "duetime", "scheudler"), &RxObservable::delay_with_mapper, DEFVAL(VNULL));

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

    ClassDB::bind_method(D_METHOD("do_while", "condition"), &RxObservable::do_while);

    ClassDB::bind_method(D_METHOD("element_at_or_default", "index", "has_default", "default_value"), &RxObservable::element_at_or_default, DEFVAL(false), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("exclusive"), &RxObservable::exclusive);

    ClassDB::bind_method(D_METHOD("expand", "mapper"), &RxObservable::expand);

    ClassDB::bind_method(D_METHOD("finally_action", "action"), &RxObservable::finally_action);

    ClassDB::bind_method(D_METHOD("find_value", "predicate", "yield_index"), &RxObservable::find_value);

    ClassDB::bind_method(D_METHOD("first", "predicate"), &RxObservable::first, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("flat_map", "mapper"), &RxObservable::flat_map, DEFVAL(CBNULL));
    ClassDB::bind_method(D_METHOD("flat_map_indexed", "mapper"), &RxObservable::flat_map_indexed, DEFVAL(CBNULL));
    ClassDB::bind_method(D_METHOD("flat_map_latest", "mapper"), &RxObservable::flat_map_latest, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("first_or_default", "predicate"), &RxObservable::first_or_default, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("fork_joinv", "sources"), &RxObservable::fork_joinv);
    RX_BIND_VARARG_METHOD(RxObservable, fork_join, OBJECT, sources)

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
    ClassDB::bind_method(D_METHOD("mergev", "sources", "max_concurrent"), &RxObservable::mergev, DEFVAL(-1));
    RX_BIND_VARARG_METHOD(RxObservable, merge, VARIANT_MAX, sources);

    ClassDB::bind_method(D_METHOD("min", "comparer"), &RxObservable::min, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("min_by", "key_mapper", "comparer"), &RxObservable::min_by, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("multicast", "subject"), &RxObservable::multicast);
    ClassDB::bind_method(D_METHOD("multicast_factory", "subject_factory", "mapper"), &RxObservable::multicast_factory, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("observe_on", "scheduler"), &RxObservable::observe_on);

    ClassDB::bind_method(D_METHOD("on_error_resume_next", "second"), &RxObservable::on_error_resume_next);

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

    ClassDB::bind_method(D_METHOD("repeat", "repeat_count"), &RxObservable::repeat, DEFVAL(-1));

    ClassDB::bind_method(D_METHOD("replay", "mapper", "buffer_size", "window", "scheduler"), &RxObservable::replay, DEFVAL(CBNULL), DEFVAL(1024ul), DEFVAL(DTZERO), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("retry", "retry_count"), &RxObservable::retry, DEFVAL(-1));

    ClassDB::bind_method(D_METHOD("sample_with_time", "sampler", "scheduler"), &RxObservable::sample_with_time, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("sample", "sampler"), &RxObservable::sample);

    ClassDB::bind_method(D_METHOD("scan", "accumulator", "seed"), &RxObservable::scan, DEFVAL(memnew(NotSet)));

    ClassDB::bind_method(D_METHOD("sequence_equal", "second", "comparer"), &RxObservable::sequence_equal, DEFVAL(CBNULL));
    ClassDB::bind_method(D_METHOD("sequence_equal_it", "iterable", "comparer"), &RxObservable::sequence_equal_iterable, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("single", "predicate"), &RxObservable::single, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("single_or_default_async", "has_default", "default_value"), &RxObservable::single_or_default_async, DEFVAL(false), DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("single_or_default", "predicate", "default_value"), &RxObservable::single_or_default, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("skip", "count"), &RxObservable::skip);

    ClassDB::bind_method(D_METHOD("skip_last", "count"), &RxObservable::skip_last);

    ClassDB::bind_method(D_METHOD("skip_last_with_time_rel", "duration", "scheduler"), &RxObservable::skip_last_with_time_rel, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("skip_last_with_time_abs", "duration", "scheduler"), &RxObservable::skip_last_with_time_abs, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("skip_last_with_time", "duration", "scheduler"), &RxObservable::skip_last_with_time, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("skip_until", "other"), &RxObservable::skip_until);

    ClassDB::bind_method(D_METHOD("skip_until_with_time_rel", "start_time", "scheduler"), &RxObservable::skip_until_with_time_rel, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("skip_until_with_time_abs", "start_time", "scheduler"), &RxObservable::skip_until_with_time_abs, DEFVAL(VNULL));
    ClassDB::bind_method(D_METHOD("skip_until_with_time", "start_time", "scheduler"), &RxObservable::skip_until_with_time, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("skip_while", "predicate"), &RxObservable::skip_while);
    ClassDB::bind_method(D_METHOD("skip_while_indexed", "predicate"), &RxObservable::skip_while_indexed);

    ClassDB::bind_method(D_METHOD("skip_with_time", "duration", "scheduler"), &RxObservable::skip_with_time, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("slice", "start", "stop", "step"), &RxObservable::slice, DEFVAL(0l), DEFVAL(INT64_MAX), DEFVAL(1l));

    ClassDB::bind_method(D_METHOD("some", "predicate"), &RxObservable::some, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("start_with", "args"), &RxObservable::start_with);

    ClassDB::bind_method(D_METHOD("subscribe_on", "scheduler"), &RxObservable::subscribe_on);

    ClassDB::bind_method(D_METHOD("sum", "key_mapper"), &RxObservable::sum, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("switch_latest"), &RxObservable::switch_latest);

    ClassDB::bind_method(D_METHOD("take", "count"), &RxObservable::take);

    ClassDB::bind_method(D_METHOD("take_last_buffer", "count"), &RxObservable::take_last_buffer);

    ClassDB::bind_method(D_METHOD("take_last_with_time", "duration", "scheduler"), &RxObservable::take_last_with_time, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("take_while", "predicate", "inclusive"), &RxObservable::take_while, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("take_while_indexed", "predicate", "inclusive"), &RxObservable::take_while_indexed, DEFVAL(false));

    ClassDB::bind_method(D_METHOD("take_with_time", "duration", "scheduler"), &RxObservable::take_with_time, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("throttle_first", "window_duration", "scheduler"), &RxObservable::throttle_first, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("time_interval", "scheduler"), &RxObservable::time_interval, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("timeout", "duetime", "other", "scheduler"), &RxObservable::timeout, DEFVAL(VNULL), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("timeout_with_mapper", "first_timeout", "timeout_duration_mapper", "other"), &RxObservable::timeout_with_mapper, DEFVAL(VNULL), DEFVAL(CBNULL), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("timestamp", "scheduler"), &RxObservable::timestamp, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("to_dict", "key_mapper", "element_mapper"), &RxObservable::to_dict, DEFVAL(CBNULL), DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("to_list"), &RxObservable::to_list);
    ClassDB::bind_method(D_METHOD("to_iterable"), &RxObservable::to_iterable);

    ClassDB::bind_method(D_METHOD("to_set", "comparer"), &RxObservable::to_set, DEFVAL(CBNULL));

    ClassDB::bind_method(D_METHOD("while_do", "condition"), &RxObservable::while_do);

    ClassDB::bind_method(D_METHOD("window_toggle", "openings", "closing_mapper"), &RxObservable::window_toggle);
    ClassDB::bind_method(D_METHOD("window", "boundaries"), &RxObservable::window);
    ClassDB::bind_method(D_METHOD("window_when", "closing_mapper"), &RxObservable::window_when);

    ClassDB::bind_method(D_METHOD("window_with_count", "count", "skip"), &RxObservable::window_with_count, DEFVAL(-1));

    ClassDB::bind_method(D_METHOD("window_with_time", "timespan", "timeshift", "scheduler"), &RxObservable::window_with_time, DEFVAL(DTZERO), DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("window_with_time_or_count", "timespan", "count", "scheduler"), &RxObservable::window_with_time_or_count, DEFVAL(VNULL));

    ClassDB::bind_method(D_METHOD("with_latest_fromv", "sources"), &RxObservable::with_latest_fromv);
    RX_BIND_VARARG_METHOD(RxObservable, with_latest_from, OBJECT, "sources")

    ClassDB::bind_method(D_METHOD("zipv", "sources"), &RxObservable::zipv);
    RX_BIND_VARARG_METHOD(RxObservable, zip, OBJECT, "sources")
    ClassDB::bind_method(D_METHOD("zip_with_iterable", "iterable"), &RxObservable::zip_with_iterable);

    /* CONNECTABLE */

    ClassDB::bind_static_method("RxObservable", D_METHOD("add_ref", "xs", "r"), &RxObservable::add_ref);
    ClassDB::bind_method(D_METHOD("ref_count"), &RxObservable::ref_count);

    /* GODOT */

    ClassDB::bind_static_method("RxObservable", D_METHOD("from_signal", "owner", "signal_name", "track_owner", "scheduler"), &RxObservable::from_signal, DEFVAL(false), DEFVAL(VNULL));
}

} // END namespace rx::wrappers