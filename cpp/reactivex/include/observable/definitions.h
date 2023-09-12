#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "internal/mapping.h"

namespace rx {

namespace observable {

struct obs {
    // case.h
    static rx_observable_t case_(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr);
    // catch.h
    static rx_observable_t catch_with_iterable_(const iterable_t& sources);
    static rx_observable_t catch_(const rx_observable_list_t& sources);
    static rx_observable_t catch_(const rx_observable_t& sources...);
    // combinelatest.h
    static rx_observable_t combine_latest_(const rx_observable_list_t& sources);
    static rx_observable_t combine_latest_(const rx_observable_t& sources...);
    // concat.h
    static rx_observable_t concat_with_iterable_(const iterable_t& sources);
    static rx_observable_t concat_(const rx_observable_list_t& sources);
    static rx_observable_t concat_(const rx_observable_t& sources...);
    // defer.h
    static rx_observable_t defer_(const observable_factory_t& factory);
    // empty.h
    static rx_observable_t empty_(const scheduler_t& scheduler = nullptr);
    // forkjoin.h
    static rx_observable_t fork_join_(const rx_observable_list_t& sources);
    static rx_observable_t fork_join_(const rx_observable_t& sources...);
    // fromiterable.h
    static rx_observable_t from_iterable_(const iterable_t& iterable, const scheduler_t& scheduler = nullptr);
    // generate.h
    static rx_observable_t generate_(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate);
    // ifthen.h
    static rx_observable_t if_then_(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr);
    // merge.h
    static rx_observable_t merge_(const rx_observable_list_t& sources);
    static rx_observable_t merge_(const rx_observable_t& sources...);
    // never.h
    static rx_observable_t never_();
    // onerrorresumenext.h
    static rx_observable_t on_error_resume_next_(const rx_observable_list_t& sources);
    static rx_observable_t on_error_resume_next_(const rx_observable_t& sources...);
    static rx_observable_t on_error_resume_next_(const RxList<stated_observable_factory_t>& sources);
    static rx_observable_t on_error_resume_next_(const stated_observable_factory_t& sources...);
    // range.h
    static rx_observable_t range_(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const scheduler_t& scheduler = nullptr);
    // returnvalue.h
    static rx_observable_t return_value_(const Variant& value, const scheduler_t& scheduler = nullptr);
    // throw.h
    static rx_observable_t throw_(const error_t& error, const scheduler_t& scheduler = nullptr);
    // timer.h
    static rx_observable_t timer_(const time_point_t& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_point_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const time_delta_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const double& duetime, const scheduler_t& scheduler = nullptr);
    static rx_observable_t timer_(const double& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr);
    // using.h
    static rx_observable_t using_(const std::function<disposable_t()>& resource_factory, const std::function<rx_observable_t(const disposable_t&)>& observable_factory);
    // withlatestfrom.h
    static rx_observable_t with_latest_from_(const rx_observable_t& parent, const rx_observable_list_t& sources);
    static rx_observable_t with_latest_from_(const rx_observable_t& parent, const rx_observable_t& sources...);
    // zip.h
    static rx_observable_t zip_(const rx_observable_list_t& sources);
    static rx_observable_t zip_(const rx_observable_t& sources...);
}; // END struct obs

// case.h
static rx_observable_t case_mapper(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr) { return obs::case_(mapper, sources, default_source); }
// catch.h
static rx_observable_t catch_with_iterable(const iterable_t& sources) { return obs::catch_with_iterable_(sources); }
static rx_observable_t catch_with(const rx_observable_list_t& sources) { return obs::catch_(sources); }
static rx_observable_t catch_with(const rx_observable_t& sources...) { return obs::catch_(sources); }
// combinelatest.h
static rx_observable_t combine_latest(const rx_observable_list_t& sources) { return obs::combine_latest_(sources); }
static rx_observable_t combine_latest(const rx_observable_t& sources...) { return obs::combine_latest_(sources); }
// concat.h
static rx_observable_t concat_with_iterable(const iterable_t& sources) { return obs::concat_with_iterable_(sources); }
static rx_observable_t concat(const rx_observable_list_t& sources) { return obs::concat_(sources); }
static rx_observable_t concat(const rx_observable_t& sources...) { return obs::concat_(sources); }
// defer.h
static rx_observable_t defer(const observable_factory_t& factory) { return obs::defer_(factory); }
// empty.h
static rx_observable_t empty(const scheduler_t& scheduler = nullptr) { return obs::empty_(scheduler); }
// forkjoin.h
static rx_observable_t fork_join(const rx_observable_list_t& sources) { return obs::fork_join_(sources); }
static rx_observable_t fork_join(const rx_observable_t& sources...) { return obs::fork_join_(sources); }
// fromiterable.h
static rx_observable_t from_iterable(const iterable_t& iterable, const scheduler_t& scheduler = nullptr) { return obs::from_iterable_(iterable, scheduler); }
static rx_observable_t from(const iterable_t& iterable, const scheduler_t& scheduler = nullptr) { return obs::from_iterable_(iterable, scheduler); }
// generate.h
static rx_observable_t generate(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate) { return obs::generate_(initial_state, condition, iterate); }
// ifthen.h
static rx_observable_t if_then(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr) { return obs::if_then_(condition, then_source, else_source); }
// merge.h
static rx_observable_t merge(const rx_observable_list_t& sources) { return obs::merge_(sources); }
static rx_observable_t merge(const rx_observable_t& sources...) { return obs::merge_(sources); }
// never.h
static rx_observable_t never() { return obs::never_(); }
// onerrorresumenext.h
static rx_observable_t on_error_resume_next(const rx_observable_list_t& sources) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const rx_observable_t& sources...) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const RxList<stated_observable_factory_t>& sources) { return obs::on_error_resume_next_(sources); }
static rx_observable_t on_error_resume_next(const stated_observable_factory_t& sources...) { return obs::on_error_resume_next_(sources); }
// range.h
static rx_observable_t range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const scheduler_t& scheduler = nullptr) { return obs::range_(start, stop, step, scheduler); }
// returnvalue.h
static rx_observable_t return_value(const Variant& value, const scheduler_t& scheduler = nullptr) { return obs::return_value_(value, scheduler); }
static rx_observable_t just(const Variant& value, const scheduler_t& scheduler = nullptr) { return obs::return_value_(value, scheduler); }
// throw.h
static rx_observable_t throw_error(const error_t& error, const scheduler_t& scheduler = nullptr) { return obs::throw_(error); }
// timer.h
static rx_observable_t timer(const time_point_t& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const time_point_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
static rx_observable_t timer(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const time_delta_t& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
static rx_observable_t timer(const double& duetime, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, scheduler); }
static rx_observable_t timer(const double& duetime, const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::timer_(duetime, period, scheduler); }
// using.h
static rx_observable_t using_resource(const std::function<disposable_t()>& resource_factory, const std::function<rx_observable_t(const disposable_t&)>& observable_factory) { return obs::using_(resource_factory, observable_factory); }
// withlatestfrom.h
static rx_observable_t with_latest_from(const rx_observable_t& parent, const rx_observable_list_t& sources) { return obs::with_latest_from_(parent, sources); }
static rx_observable_t with_latest_from(const rx_observable_t& parent, const rx_observable_t& sources...) { return obs::with_latest_from_(parent, sources); }
// zip.h
static rx_observable_t zip(const rx_observable_list_t& sources) { return obs::zip_(sources); }
static rx_observable_t zip(const rx_observable_t& sources...) { return obs::zip_(sources); }

namespace op {

struct ops {
    // _all.h
    static observable_op_t all_(const predicate_t<Variant>& predicate);
    // _amb.h
    static observable_op_t amb_(const rx_observable_t& right_source);
    // _asobservable.h
    static observable_op_t as_observable_();
    // _average.h
    static observable_op_t average_(const mapper_t<double, Variant>& key_mapper = nullptr);
    // _catch.h
    static observable_op_t catch_(const rx_observable_t& handler);
    static observable_op_t catch_(const std::function<rx_observable_t(const error_t&, const rx_observable_t&)>& handler);
    // _combinelatest.h
    static observable_op_t combine_latest_(const rx_observable_list_t& others);
    static observable_op_t combine_latest_(const rx_observable_t& others...);
    // _concat.h
    static observable_op_t concat_(const rx_observable_list_t& sources);
    static observable_op_t concat_(const rx_observable_t& others...);
    // _contains.h
    static observable_op_t contains_(const Variant& value, const comparer_t<Variant>& comparer = nullptr);
    // _count.h
    static observable_op_t count_(const predicate_t<Variant>& predicate = nullptr);
    // _debounce.h
    static observable_op_t debounce_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    static observable_op_t throttle_with_mapper_(const mapper_t<rx_observable_t, Variant>& throttle_duration_mapper);
    // _defaultifempty.h
    static observable_op_t default_if_empty_(const Variant& default_value = VNULL);
    // _dematerialize.h
    static observable_op_t dematerialize_();
    // _distinct.h
    static observable_op_t distinct_(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr);
    // _distinctuntilchanged.h
    static observable_op_t distinct_until_changed_(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr);
    // _do.h
    static observable_op_t do_action_(const on_next_t& on_next = nullptr, const on_error_t& on_error = nullptr, const on_completed_t& on_completed = nullptr);
    static observable_op_t do_(const observer_t& observer);
    static rx_observable_t do_after_next(const rx_observable_t& source, const on_next_t& after_next);
    static rx_observable_t do_on_subscribe(const rx_observable_t& source, const action_t& on_subscribe);
    static rx_observable_t do_on_dispose(const rx_observable_t& source, const dispose_t& on_dispose);
    static rx_observable_t do_on_terminate(const rx_observable_t& source, const action_t& on_terminate);
    static rx_observable_t do_after_terminate(const rx_observable_t& source, const action_t& after_terminate);
    static observable_op_t do_finally_(const action_t& finally_action);
    // _elementatordefault.h
    static observable_op_t element_at_or_default_(uint64_t index, bool has_default = false, const Variant& default_value = VNULL);
    // _exclusive.h
    static observable_op_t exclusive_();
    // _expand.h
    static observable_op_t expand_(const mapper_t<rx_observable_t, Variant>& mapper);
    // _filter.h
    static observable_op_t filter_(const predicate_t<Variant>& predicate);
    static observable_op_t filter_indexed_(const predicate_indexed_t<Variant>& predicate = nullptr);
    // _finallyaction.h
    static observable_op_t finally_action_(const action_t& action);
    // _find.h
    static observable_op_t find_value_(const predicate_t<Variant, uint64_t, rx_observable_t>& predicate, bool yield_index);
    // _first.h
    static observable_op_t first_(const predicate_t<Variant>& predicate = nullptr);
    // _firstordefault.h
    static observable_op_t first_or_default_async_(bool has_default = false, const Variant& default_value = VNULL);
    static observable_op_t first_or_default_(const predicate_t<Variant>& predicate = nullptr, const Variant& default_value = VNULL);
    // _forkjoin.h
    static observable_op_t fork_join_(const rx_observable_list_t& args);
    static observable_op_t fork_join_(const rx_observable_t& args...);
    // _groupby.h
    static observable_op_t group_by_(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr, const mapper_t<rx_subject_t>& subject_mapper = nullptr);
    // _groupedbyuntil.h
    static observable_op_t group_by_until_(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<rx_observable_t, rx_grouped_observable_t>& duration_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr, const mapper_t<rx_subject_t>& subject_mapper = nullptr);
    // _groupjoin.h
    static observable_op_t group_join_(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper);
    // _ignoreelements.h
    static observable_op_t ignore_elements_();
    // _isempty.h
    static observable_op_t is_empty_();
    // _join.h
    static observable_op_t join_(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper);
    // _last.h
    static observable_op_t last_(const predicate_t<Variant>& predicate = nullptr);
    // _lastordefault.h
    static rx_observable_t last_or_default_async_(const rx_observable_t& source, bool has_default = false, const Variant& default_value = VNULL);
    static observable_op_t last_or_default_(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr);
    // _map.h
    static observable_op_t map_(const mapper_t<Variant, Variant>& mapper = nullptr);
    static observable_op_t map_indexed_(const mapper_indexed_t<Variant, Variant>& mapper = nullptr);
    // _materialize.h
    static observable_op_t materialize_();
    // _max.h
    static observable_op_t max_(const comparer_t<Variant>& comparer = nullptr);
    // _maxby.h
    static observable_op_t max_by_(const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer = nullptr);
    // _merge.h
    static observable_op_t merge_(const rx_observable_list_t& sources, int64_t max_concurrent = -1);
    static observable_op_t merge_(int64_t max_concurrent, const rx_observable_t& sources...);
    static observable_op_t merge_all_();
    // _min.h
    static observable_op_t min_(const comparer_t<Variant>& comparer = nullptr);
    // _minby.h
    static rx_observable_t extrema_by(const rx_observable_t& source, const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer);
    static observable_op_t min_by_(const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer = nullptr);
    // _multicast.h
    static observable_op_t multicast_(const rx_subject_t& subject);
    static observable_op_t multicast_(const rx_subject_factory_t& subject_factory, const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr);
    // _observeon.h
    static observable_op_t observe_on_(const scheduler_t& scheduler);
    // _onerrorresumenext.h
    static observable_op_t on_error_resume_next_(const rx_observable_t& second);
    // _pairwise.h
    static observable_op_t pairwise_();
    // _partition.h
    static partition_op_t partition_(const predicate_t<Variant>& predicate);
    static partition_op_t partition_indexed_(const predicate_indexed_t<Variant>& predicate);
    // _pluck.h
    static observable_op_t pluck_(const Variant& key);
    static observable_op_t pluck_attr_(const String& prop);
    // _publish.h
    static observable_op_t publish_(const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr);
    static observable_op_t share_();
    // _publishvalue.h
    static observable_op_t publish_value_(const Variant& initial_value, const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr);
    // _reduce.h
    static observable_op_t reduce_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value());
    // _scan.h
    static observable_op_t scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value());
    // _some.h
    static observable_op_t some_(const predicate_t<Variant>& predicate = nullptr);
    // _take.h
    static observable_op_t take_(uint64_t count);
    // _timestamp.h
    static observable_op_t timestamp_(const scheduler_t& scheduler = nullptr);
    // _withlatestfrom.h
    static observable_op_t with_latest_from_(const rx_observable_list_t& sources);
    static observable_op_t with_latest_from_(const rx_observable_t& others...);
    // _zip.h
    static observable_op_t zip_(const rx_observable_list_t& sources);
    static observable_op_t zip_(const rx_observable_t& others...);
    static observable_op_t zip_with_iterable_(const iterable_t& seq);

}; // END struct ops

// _all.h
static observable_op_t all(const predicate_t<Variant>& predicate) { return ops::all_(predicate); }
// _amb.h
static observable_op_t amb(const rx_observable_t& right_source) { return ops::amb_(right_source); }
// _asobservable.h
static observable_op_t as_observable() { return ops::as_observable_(); }
// _average.h
static observable_op_t average(const mapper_t<double, Variant>& key_mapper = nullptr) { return ops::average_(key_mapper); }
// _catch.h
static observable_op_t catch_with(const rx_observable_t& handler) { return ops::catch_(handler); }
static observable_op_t catch_with(const std::function<rx_observable_t(const error_t&, const rx_observable_t&)>& handler) { return ops::catch_(handler); }
// _combinelatest.h
static observable_op_t combine_latest(const rx_observable_list_t& others) { return ops::combine_latest_(others); }
static observable_op_t combine_latest(const rx_observable_t& others...) { return ops::combine_latest_(others); }
// _concat.h
static observable_op_t concat(const rx_observable_list_t& sources) { return ops::concat_(sources); }
static observable_op_t concat(const rx_observable_t& others...) { return ops::concat_(others); }
// _contains.h
static observable_op_t contains(const Variant& value, const comparer_t<Variant>& comparer = nullptr) { return ops::contains_(value, comparer); }
// _count.h
static observable_op_t count(const predicate_t<Variant>& predicate = nullptr) { return ops::count_(predicate); }
// _debounce.h
static observable_op_t debounce(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return ops::debounce_(duetime, scheduler); }
static observable_op_t throttle_with_mapper(const mapper_t<rx_observable_t, Variant>& throttle_duration_mapper) { return ops::throttle_with_mapper_(throttle_duration_mapper); }
// _defaultifempty.h
static observable_op_t default_if_empty(const Variant& default_value = VNULL) { return ops::default_if_empty_(default_value); }
// _dematerialize.h
static observable_op_t dematerialize() { return ops::dematerialize_(); }
// _distinct.h
static observable_op_t distinct(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr) { return ops::distinct_(key_mapper, comparer); }
// _distinctuntilchanged.h
static observable_op_t distinct_until_changed(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr) { return ops::distinct_until_changed_(key_mapper, comparer); }
// _do.h
static observable_op_t do_action(const on_next_t& on_next = nullptr, const on_error_t& on_error = nullptr, const on_completed_t& on_completed = nullptr) { return ops::do_action_(on_next, on_error, on_completed); }
static observable_op_t do_observer(const observer_t& observer) { return ops::do_(observer); }
static rx_observable_t do_after_next(const rx_observable_t& source, const on_next_t& after_next) { return ops::do_after_next(source, after_next); }
static rx_observable_t do_on_subscribe(const rx_observable_t& source, const action_t& on_subscribe) { return ops::do_on_subscribe(source, on_subscribe); }
static rx_observable_t do_on_dispose(const rx_observable_t& source, const dispose_t& on_dispose) { return ops::do_on_dispose(source, on_dispose); }
static rx_observable_t do_on_terminate(const rx_observable_t& source, const action_t& on_terminate) { return ops::do_on_dispose(source, on_terminate); }
static rx_observable_t do_after_terminate(const rx_observable_t& source, const action_t& after_terminate) { return ops::do_on_dispose(source, after_terminate); }
static observable_op_t do_finally(const action_t& finally_action) { return ops::do_finally_(finally_action); }
// _elementatordefault.h
static observable_op_t element_at_or_default_(uint64_t index, bool has_default = false, const Variant& default_value = VNULL) { return ops::element_at_or_default_(index, has_default, default_value); }
// _exclusive.h
static observable_op_t exclusive() { return ops::exclusive_(); }
// _expand.h
static observable_op_t expand(const mapper_t<rx_observable_t, Variant>& mapper) { return ops::expand_(mapper); }
// _filter.h
static observable_op_t filter(const predicate_t<Variant>& predicate) { return ops::filter_(predicate); }
static observable_op_t filter_indexed(const predicate_indexed_t<Variant>& predicate = nullptr) { return ops::filter_indexed_(predicate); }
// _finallyaction.h
static observable_op_t finally_action(const action_t& action) { return ops::finally_action_(action); }
// _find.h
static observable_op_t find_value(const predicate_t<Variant, uint64_t, rx_observable_t>& predicate, bool yield_index) { return ops::find_value_(predicate, yield_index); }
// _first.h
static observable_op_t first(const predicate_t<Variant>& predicate = nullptr) { return ops::filter_(predicate); }
// _firstordefault.h
static observable_op_t first_or_default_async(bool has_default = false, const Variant& default_value = VNULL) { return ops::first_or_default_async_(has_default, default_value); }
static observable_op_t first_or_default(const predicate_t<Variant>& predicate = nullptr, const Variant& default_value = VNULL) { return ops::first_or_default_(predicate, default_value); }
// _forkjoin.h
static observable_op_t fork_join(const rx_observable_list_t& args) { return ops::fork_join_(args); }
static observable_op_t fork_join(const rx_observable_t& args...) { return ops::fork_join_(args); }
// _groupby.h
static observable_op_t group_by(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr, const mapper_t<rx_subject_t>& subject_mapper = nullptr) { return ops::group_by_(key_mapper, element_mapper, subject_mapper); }
// _groupedbyuntil.h
static observable_op_t group_by_until(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<rx_observable_t, rx_grouped_observable_t>& duration_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr, const mapper_t<rx_subject_t>& subject_mapper = nullptr) { return ops::group_by_until_(key_mapper, duration_mapper, element_mapper, subject_mapper); }
// _groupjoin.h
static observable_op_t group_join(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper) { return ops::group_join_(right, left_duration_mapper, right_duration_mapper); }
// _ignoreelements.h
static observable_op_t ignore_elements() { return ops::ignore_elements_(); }
// _isempty.h
static observable_op_t is_empty() { return ops::is_empty_(); }
// _join.h
static observable_op_t join(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper) { return ops::join_(right, left_duration_mapper, right_duration_mapper); }
// _last.h
static observable_op_t last(const predicate_t<Variant>& predicate = nullptr) { return ops::last_(predicate); }
// _lastordefault.h
static rx_observable_t last_or_default_async(const rx_observable_t& source, bool has_default = false, const Variant& default_value = VNULL) { return ops::last_or_default_async_(source, has_default, default_value); }
static observable_op_t last_or_default(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr) { return ops::last_or_default_(default_value, predicate); }
// _map.h
static observable_op_t map(const mapper_t<Variant, Variant>& mapper = nullptr) { return ops::map_(mapper); }
static observable_op_t map_indexed(const mapper_indexed_t<Variant, Variant>& mapper = nullptr) { return ops::map_indexed_(mapper); }
// _materialize.h
static observable_op_t materialize() { return ops::materialize_(); }
// _max.h
static observable_op_t max(const comparer_t<Variant>& comparer = nullptr) { return ops::max_(comparer); }
// _maxby.h
static observable_op_t max_by(const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer = nullptr) { return ops::max_by_(key_mapper, comparer); }
// _merge.h
static observable_op_t merge(const rx_observable_list_t& sources, int64_t max_concurrent = -1) { return ops::merge_(sources, max_concurrent); }
static observable_op_t merge(int64_t max_concurrent, const rx_observable_t& sources...) { return ops::merge_(max_concurrent, sources); }
static observable_op_t merge_all() { return ops::merge_all_(); }
// _min.h
static observable_op_t min(const comparer_t<Variant>& comparer = nullptr) { return ops::min_(comparer); }
// _minby.h
static rx_observable_t extrema_by(const rx_observable_t& source, const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer) { return ops::extrema_by(source, key_mapper, comparer); }
static observable_op_t min_by(const mapper_t<Variant, Variant>& key_mapper, const sub_comparer_t<Variant>& comparer = nullptr) { return ops::min_by_(key_mapper, comparer); }
// _multicast.h
static observable_op_t multicast(const rx_subject_t& subject) { return ops::multicast_(subject); }
static observable_op_t multicast(const rx_subject_factory_t& subject_factory, const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr) { return ops::multicast_(subject_factory, mapper); }
// _observeon.h
static observable_op_t observe_on(const scheduler_t& scheduler) { return ops::observe_on_(scheduler); }
// _onerrorresumenext.h
static observable_op_t on_error_resume_next(const rx_observable_t& second) { return ops::on_error_resume_next_(second); }
// _pairwise.h
static observable_op_t pairwise() { return ops::pairwise_(); }
// _partition.h
static partition_op_t partition(const predicate_t<Variant>& predicate) { return ops::partition_(predicate); }
static partition_op_t partition_indexed(const predicate_indexed_t<Variant>& predicate) { return ops::partition_indexed_(predicate); }
// _pluck.h
static observable_op_t pluck(const Variant& key) { return ops::pluck_(key); }
static observable_op_t pluck_attr(const String& prop) { return ops::pluck_attr_(prop); }
// _publish.h
static observable_op_t publish(const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr) { return ops::publish_(mapper); }
static observable_op_t share() { return ops::share_(); }
// _publishvalue.h
static observable_op_t publish_value(const Variant& initial_value, const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr) { return ops::publish_value_(initial_value, mapper); }
// _reduce.h
static observable_op_t reduce(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return ops::reduce_(accumulator, seed); }
// _scan.h
static observable_op_t scan(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return ops::scan_(accumulator, seed); }
// _some.h
static observable_op_t some(const predicate_t<Variant>& predicate = nullptr) { return ops::some_(predicate); }
// _take.h
static observable_op_t take(uint64_t count) { return ops::take_(count); }
// _timestamp.h
static observable_op_t timestamp(const scheduler_t& scheduler = nullptr) { return ops::timestamp_(scheduler); }
// _withlatestfrom.h
static observable_op_t with_latest_from(const rx_observable_list_t& sources) { return ops::with_latest_from_(sources); }
static observable_op_t with_latest_from(const rx_observable_t& sources...) { return ops::with_latest_from_(sources); }
// _zip.h
static observable_op_t zip(const rx_observable_list_t& sources) { return ops::zip_(sources); }
static observable_op_t zip(const rx_observable_t& others...) { return ops::zip_(others); }
static observable_op_t zip_with_iterable(const iterable_t& seq) { return ops::zip_with_iterable_(seq); }

} // END namespace op

struct connectable {

    static rx_observable_t add_ref(const rx_observable_t& xs, const rx_rc_disposable_t& r);
    static connectable_op_t ref_count_();

}; // END struct connectable

static rx_observable_t add_ref(const rx_observable_t& xs, const rx_rc_disposable_t& r) { return connectable::add_ref(xs, r); }
static connectable_op_t ref_count() { return connectable::ref_count_(); }


} // END namespace observable
} // END namespace rx::observable

#endif // RX_OBSERVABLE_DEFINITIONS_H