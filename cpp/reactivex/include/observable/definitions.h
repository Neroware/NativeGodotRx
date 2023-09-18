#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "internal/mapping.h"

namespace rx {

namespace observable {

struct obs {
    // amb.h
    static rx_observable_t amb_(const rx_observable_list_t& sources);
    static rx_observable_t amb_(const rx_observable_t& sources...);
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
    // create.h
    static rx_observable_t create_(const subscription_t& subscribe = nullptr);
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
    // generatewithrelativetime.h
    static rx_observable_t generate_with_relative_time_(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate, const mapper_t<time_delta_t, Variant>& time_mapper);
    // ifthen.h
    static rx_observable_t if_then_(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr);
    // interval.h
    static rx_observable_t interval_(const time_delta_t& period, const scheduler_t& scheduler = nullptr);
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
    // repeat.h
    static rx_observable_t repeat_(const Variant& value, int64_t repeat_count = -1);
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

// amb.h
static rx_observable_t amb(const rx_observable_list_t& sources) { return obs::amb_(sources); }
static rx_observable_t amb(const rx_observable_t& sources...) { return obs::amb_(sources); }
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
// create.h
static rx_observable_t create(const subscription_t& subscribe = nullptr) { return obs::create_(subscribe); }
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
// generatewithrelativetime.h
static rx_observable_t generate_with_relative_time(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate, const mapper_t<time_delta_t, Variant>& time_mapper) { return obs::generate_with_relative_time_(initial_state, condition, iterate, time_mapper); }
// ifthen.h
static rx_observable_t if_then(const predicate_t<>& condition, const rx_observable_t& then_source, const rx_observable_t& else_source = nullptr) { return obs::if_then_(condition, then_source, else_source); }
// interval.h
static rx_observable_t interval(const time_delta_t& period, const scheduler_t& scheduler = nullptr) { return obs::interval_(period, scheduler); }
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
// repeat.h
static rx_observable_t repeat(const Variant& value, int64_t repeat_count = -1) { return obs::repeat_(value, repeat_count); }
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
    // _buffer.h
    static observable_op_t buffer_(const rx_observable_t& boundaries);
    static observable_op_t buffer_when_(const mapper_t<rx_observable_t>& closing_mapper);
    static observable_op_t buffer_toggle_(const rx_observable_t& openings, const mapper_t<rx_observable_t, Variant>& closing_mapper);
    static observable_op_t buffer_with_count_(int64_t count, int64_t skip = -1);
    // _bufferwithtime.h
    static observable_op_t buffer_with_time_(const time_delta_t& timespan, const time_delta_t& timeshift = DELTA_ZERO, const scheduler_t& scheduler = nullptr);
    // _bufferwithtimeorcount.h
    static observable_op_t buffer_with_time_or_count_(const time_delta_t& timespan, int64_t count, const scheduler_t& scheduler = nullptr);
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
    // _delay.h
    static rx_observable_t observable_delay_timespan(const rx_observable_t& source, const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    static observable_op_t delay_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    // _delaysubscription.h
    static observable_op_t delay_subscription_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr);
    // _delaywithmapper.h
    static observable_op_t delay_with_mapper_(const rx_observable_t& subscription_delay = nullptr, const mapper_t<rx_observable_t, Variant>& subscription_delay_mapper = nullptr, const rx_observable_t& delay_duration_mapper = nullptr);
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
    // _whiledo.h
    static observable_op_t do_while_(const predicate_t<rx_observable_t>& condition);
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
    // _flatmap.h
    static observable_op_t flat_map_(const mapper_t<rx_observable_t, Variant>& mapper = nullptr);
    static observable_op_t flat_map_indexed_(const mapper_indexed_t<rx_observable_t, Variant>& mapper = nullptr);
    static observable_op_t flat_map_latest_(const mapper_t<rx_observable_t, Variant>& mapper = nullptr);
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
    // _repeat.h
    static observable_op_t repeat_(int64_t repeat_count = -1);
    // _replay.h
    static observable_op_t replay_(const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr, uint64_t buffer_size = 1024ul, const time_delta_t& window = DELTA_ZERO, const scheduler_t scheduler = nullptr);
    // _retry.h
    static observable_op_t retry_(int64_t retry_count = -1);
    // _sample.h
    static rx_observable_t sample_observable(const rx_observable_t& source, const rx_observable_t& sampler);
    static observable_op_t sample_(const time_delta_t& sampler, const scheduler_t& scheduler = nullptr);
    static observable_op_t sample_(const rx_observable_t& sampler);
    // _scan.h
    static observable_op_t scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value());
    // _sequenceequal.h
    static observable_op_t sequence_equal_(const rx_observable_t& second, const comparer_t<Variant>& comparer = nullptr);
    static observable_op_t sequence_equal_(const iterable_t& second, const comparer_t<Variant>& comparer = nullptr);
    // _single.h
    static observable_op_t single_(const predicate_t<Variant>& predicate = nullptr);
    // _singleordefault.h
    static observable_op_t single_or_default_async_(bool has_default = false, const Variant& default_value = VNULL);
    static observable_op_t single_or_default_(const predicate_t<Variant>& predicate, const Variant& default_value = VNULL);
    // _skip.h
    static observable_op_t skip_(uint64_t count);
    // _skiplast.h
    static observable_op_t skip_last_(uint64_t count);
    // _skiplastwithtime.h
    static observable_op_t skip_last_with_time_(const time_point_t& duration, const scheduler_t& scheduler = nullptr);
    static observable_op_t skip_last_with_time_(const time_delta_t& duration, const scheduler_t& scheduler = nullptr);
    // _skipuntil.h
    static observable_op_t skip_until_(const rx_observable_t& other);
    // _skipuntilwithtime.h
    static observable_op_t skip_until_with_time_(const time_delta_t& start_time, const scheduler_t& scheduler = nullptr);
    static observable_op_t skip_until_with_time_(const time_point_t& start_time, const scheduler_t& scheduler = nullptr);
    // _skipwhile.h
    static observable_op_t skip_while_(const predicate_t<Variant>& predicate);
    static observable_op_t skip_while_indexed_(const predicate_indexed_t<Variant>& predicate);
    // _skipwithtime.h
    static observable_op_t skip_with_time_(const time_delta_t& duration, const scheduler_t& scheduler = nullptr);
    // _slice.h
    static observable_op_t slice_(int64_t start = 0l, int64_t stop = INT64_MAX, int64_t step = 1l);
    // _some.h
    static observable_op_t some_(const predicate_t<Variant>& predicate = nullptr);
    // _startwith.h
    static observable_op_t start_with_(const RxList<Variant>& args);
    static observable_op_t start_with_(const Variant& args...);
    // _subscribeon.h
    static observable_op_t subscribe_on_(const scheduler_t& scheduler);
    // _sum.h
    static observable_op_t sum_(const mapper_t<float, Variant>& key_mapper = nullptr);
    // _switchlatest.h
    static observable_op_t switch_latest_();
    // _take.h
    static observable_op_t take_(uint64_t count);
    // _takelast.h
    static observable_op_t take_last_(uint64_t count);
    // _takelastwithbuffer.h
    static observable_op_t take_last_buffer_(uint64_t count);
    // _takelastwithtime.h
    static observable_op_t take_last_with_time_(const time_delta_t& duration, const scheduler_t& scheduler = nullptr);
    // _takeuntil.h
    static observable_op_t take_until_(const rx_observable_t& other);
    // _takeuntilwithtime.h
    static observable_op_t take_until_with_time_(const time_delta_t& end_time, const scheduler_t& scheduler = nullptr);
    static observable_op_t take_until_with_time_(const time_point_t& end_time, const scheduler_t& scheduler = nullptr);
    // _takewhile.h
    static observable_op_t take_while_(const predicate_t<Variant>& predciate, bool inclusive = false);
    static observable_op_t take_while_indexed_(const predicate_indexed_t<Variant>& predciate, bool inclusive = false);
    // _takewithtime.h
    static observable_op_t take_with_time_(const time_delta_t& duration, const scheduler_t& scheduler = nullptr);
    // _throttlefirst.h
    static observable_op_t throttle_first_(const time_delta_t& window_duration, const scheduler_t& scheduler = nullptr);
    // _timeinterval.h
    static observable_op_t time_interval_(const scheduler_t& scheduler = nullptr);
    // _timeout.h
    static observable_op_t timeout_(const time_delta_t& duetime, const rx_observable_t& other = nullptr, const scheduler_t& scheduler = nullptr);
    static observable_op_t timeout_(const time_point_t& duetime, const rx_observable_t& other = nullptr, const scheduler_t& scheduler = nullptr);
    // _timeoutwithmapper.h
    static observable_op_t timeout_with_mapper_(const rx_observable_t& first_timeout = nullptr, const mapper_t<rx_observable_t, Variant>& timeout_duration_mapper = nullptr, const rx_observable_t& other = nullptr);
    // _timestamp.h
    static observable_op_t timestamp_(const scheduler_t& scheduler = nullptr);
    // _todict.h
    static observable_op_t to_dict_(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr);
    // _tolist.h
    static observable_op_t to_list_();
    static observable_op_t to_iterable_();
    // _toset.h
    static observable_op_t to_set_(const comparer_t<Variant>& comparer = nullptr);
    // _whiledo.h
    static observable_op_t while_do_(const predicate_t<rx_observable_t>& condition);
    // _window.h
    static observable_op_t window_toggle_(const rx_observable_t& openings, const mapper_t<rx_observable_t, Variant>& closing_mapper);
    static observable_op_t window_(const rx_observable_t& boundaries);
    static observable_op_t window_when_(const mapper_t<rx_observable_t>& closing_mapper);
    // _windowwithcount.h
    static observable_op_t window_with_count_(int64_t count, int64_t skip = -1);
    // _windowwithtime.h
    static observable_op_t window_with_time_(const time_delta_t& timespan, const time_delta_t& timeshift = DELTA_ZERO, const scheduler_t& scheduler = nullptr);
    // _windowwithtimeorcount.h
    static observable_op_t window_with_time_or_count_(const time_delta_t& timespan, int64_t count, const scheduler_t& scheduler = nullptr);
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
// _buffer.h
static observable_op_t buffer(const rx_observable_t& boundaries) { return ops::buffer_(boundaries); }
static observable_op_t buffer_when(const mapper_t<rx_observable_t>& closing_mapper) { return ops::buffer_when_(closing_mapper); }
static observable_op_t buffer_toggle(const rx_observable_t& openings, const mapper_t<rx_observable_t, Variant>& closing_mapper) { return ops::buffer_toggle_(openings, closing_mapper); }
static observable_op_t buffer_with_count(int64_t count, int64_t skip = -1) { return ops::buffer_with_count_(count, skip); }
// _bufferwithtime.h
static observable_op_t buffer_with_time(const time_delta_t& timespan, const time_delta_t& timeshift = DELTA_ZERO, const scheduler_t& scheduler = nullptr) { return ops::buffer_with_time_(timespan, timeshift, scheduler); }
// _bufferwithtimeorcount.h
static observable_op_t buffer_with_time_or_count(const time_delta_t& timespan, int64_t count, const scheduler_t& scheduler = nullptr) { return ops::buffer_with_time_or_count_(timespan, count, scheduler); }
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
// _delay.h
static rx_observable_t observable_delay_timespan(const rx_observable_t& source, const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return ops::observable_delay_timespan(source, duetime, scheduler); }
static observable_op_t delay(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return ops::delay_(duetime, scheduler); }
// _delaysubscription.h
static observable_op_t delay_subscription(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return ops::delay_subscription_(duetime, scheduler); }
// _delaywithmapper.h
static observable_op_t delay_with_mapper(const rx_observable_t& subscription_delay = nullptr, const mapper_t<rx_observable_t, Variant>& subscription_delay_mapper = nullptr, const rx_observable_t& delay_duration_mapper = nullptr) { return ops::delay_with_mapper_(subscription_delay, subscription_delay_mapper, delay_duration_mapper); }
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
// _whiledo.h
static observable_op_t do_while(const predicate_t<rx_observable_t>& condition) { return ops::do_while_(condition); }
// _elementatordefault.h
static observable_op_t element_at_or_default(uint64_t index, bool has_default = false, const Variant& default_value = VNULL) { return ops::element_at_or_default_(index, has_default, default_value); }
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
// _flatmap.h
static observable_op_t flat_map(const mapper_t<rx_observable_t, Variant>& mapper = nullptr) { return ops::flat_map_(mapper); }
static observable_op_t flat_map_indexed(const mapper_indexed_t<rx_observable_t, Variant>& mapper = nullptr) { return ops::flat_map_indexed_(mapper); }
static observable_op_t flat_map_latest(const mapper_t<rx_observable_t, Variant>& mapper = nullptr) { return ops::flat_map_latest_(mapper); }
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
// _repeat.h
static observable_op_t repeat(int64_t repeat_count = -1) { return ops::repeat_(repeat_count); }
// _replay.h
static observable_op_t replay(const mapper_t<rx_observable_t, rx_observable_t>& mapper, int64_t buffer_size = -1, const time_delta_t& window = DELTA_ZERO, const scheduler_t scheduler = nullptr) { return ops::replay_(mapper, buffer_size, window, scheduler); }
// _retry.h
static observable_op_t retry(int64_t retry_count = -1) { return ops::retry_(retry_count); }
// _sample.h
static rx_observable_t sample_observable(const rx_observable_t& source, const rx_observable_t& sampler) { return ops::sample_observable(source, sampler); }
static observable_op_t sample(const time_delta_t& sampler, const scheduler_t& scheduler = nullptr) { return ops::sample_(sampler, scheduler); }
static observable_op_t sample(const rx_observable_t& sampler) { return ops::sample_(sampler); }
// _scan.h
static observable_op_t scan(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return ops::scan_(accumulator, seed); }
// _sequenceequal.h
static observable_op_t sequence_equal(const rx_observable_t& second, const comparer_t<Variant>& comparer = nullptr) { return ops::sequence_equal_(second, comparer); }
static observable_op_t sequence_equal(const iterable_t& second, const comparer_t<Variant>& comparer = nullptr) { return ops::sequence_equal_(second, comparer); }
// _single.h
static observable_op_t single(const predicate_t<Variant>& predicate = nullptr) { return ops::single_(predicate); }
// _singleordefault.h
static observable_op_t single_or_default_async(bool has_default = false, const Variant& default_value = VNULL) { return ops::single_or_default_async_(has_default, default_value); }
static observable_op_t single_or_default(const predicate_t<Variant>& predicate, const Variant& default_value = VNULL) { return ops::single_or_default_(predicate, default_value); }
// _skip.h
static observable_op_t skip(uint64_t count) { return ops::skip_(count); }
// _skiplast.h
static observable_op_t skip_last(uint64_t count) { return ops::skip_last_(count); }
// _skiplastwithtime.h
static observable_op_t skip_last_with_time(const time_delta_t& duration, const scheduler_t& scheduler = nullptr) { return ops::skip_last_with_time_(duration, scheduler); }
// _skipuntil.h
static observable_op_t skip_until(const rx_observable_t& other) { return ops::skip_until_(other); }
// _skipuntilwithtime.h
static observable_op_t skip_until_with_time(const time_delta_t& start_time, const scheduler_t& scheduler = nullptr) { return ops::skip_until_with_time_(start_time, scheduler); }
static observable_op_t skip_until_with_time_(const time_point_t& start_time, const scheduler_t& scheduler = nullptr) { return ops::skip_until_with_time_(start_time, scheduler); }
// _slice.h
static observable_op_t slice(int64_t start = 0l, int64_t stop = INT64_MAX, int64_t step = 1l) { return ops::slice_(start, stop, step); }
// _some.h
static observable_op_t some(const predicate_t<Variant>& predicate = nullptr) { return ops::some_(predicate); }
// _startwith.h
static observable_op_t start_with(const RxList<Variant>& args) { return ops::start_with_(args); }
static observable_op_t start_with(const Variant& args...) { return ops::start_with_(args); }
// _subscribeon.h
static observable_op_t subscribe_on(const scheduler_t& scheduler) { return ops::subscribe_on_(scheduler); }
// _sum.h
static observable_op_t sum(const mapper_t<float, Variant>& key_mapper = nullptr) { return ops::sum_(key_mapper); }
// _switchlatest.h
static observable_op_t switch_latest() { return ops::switch_latest_(); }
// _take.h
static observable_op_t take(uint64_t count) { return ops::take_(count); }
 // _takelast.h
static observable_op_t take_last(uint64_t count) { return ops::take_last_(count); }
// _takelastwithbuffer.h
static observable_op_t take_last_buffer(uint64_t count) { return ops::take_last_buffer_(count); }
// _takelastwithtime.h
static observable_op_t take_last_with_time(const time_delta_t& duration, const scheduler_t& scheduler = nullptr) { return ops::take_last_with_time_(duration, scheduler); }
// _takeuntil.h
static observable_op_t take_until(const rx_observable_t& other) { return ops::take_until_(other); }
// _takeuntilwithtime.h
static observable_op_t take_until_with_time(const time_delta_t& end_time, const scheduler_t& scheduler = nullptr) { return ops::take_until_with_time_(end_time, scheduler); }
static observable_op_t take_until_with_time(const time_point_t& end_time, const scheduler_t& scheduler = nullptr) { return ops::take_until_with_time_(end_time, scheduler); }
// _takewhile.h
static observable_op_t take_while(const predicate_t<Variant>& predicate, bool inclusive = false) { return ops::take_while_(predicate, inclusive); }
static observable_op_t take_while_indexed(const predicate_indexed_t<Variant>& predicate, bool inclusive = false) { return ops::take_while_indexed_(predicate, inclusive); }
// _takewithtime.h
static observable_op_t take_with_time(const time_delta_t& duration, const scheduler_t& scheduler = nullptr) { return ops::take_with_time_(duration, scheduler); }
// _throttlefirst.h
static observable_op_t throttle_first(const time_delta_t& window_duration, const scheduler_t& scheduler = nullptr) { return ops::throttle_first_(window_duration, scheduler); }
// _timeinterval.h
static observable_op_t time_interval(const scheduler_t& scheduler = nullptr) { return ops::time_interval_(scheduler); }
// _timeout.h
static observable_op_t timeout(const time_delta_t& duetime, const rx_observable_t& other = nullptr, const scheduler_t& scheduler = nullptr) { return ops::timeout_(duetime, other, scheduler); }
static observable_op_t timeout(const time_point_t& duetime, const rx_observable_t& other = nullptr, const scheduler_t& scheduler = nullptr) { return ops::timeout_(duetime, other, scheduler); }
// _timeoutwithmapper.h
static observable_op_t timeout_with_mapper(const rx_observable_t& first_timeout = nullptr, const mapper_t<rx_observable_t, Variant>& timeout_duration_mapper = nullptr, const rx_observable_t& other = nullptr) { return ops::timeout_with_mapper_(first_timeout, timeout_duration_mapper, other); }
// _timestamp.h
static observable_op_t timestamp(const scheduler_t& scheduler = nullptr) { return ops::timestamp_(scheduler); }
// _todict.h
static observable_op_t to_dict(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr) { return ops::to_dict_(key_mapper, element_mapper); }
// _tolist.h
static observable_op_t to_list() { return ops::to_list_(); }
static observable_op_t to_iterable() { return ops::to_iterable_(); }
// _toset.h
static observable_op_t to_set(const comparer_t<Variant>& comparer = nullptr) { return ops::to_set_(comparer); }
// _whiledo.h
static observable_op_t while_do(const predicate_t<rx_observable_t>& condition) { return ops::while_do_(condition); }
// _window.h
static observable_op_t window_toggle(const rx_observable_t& openings, const mapper_t<rx_observable_t, Variant>& closing_mapper) { return ops::window_toggle_(openings, closing_mapper); }
static observable_op_t window(const rx_observable_t& boundaries) { return ops::window_(boundaries); }
static observable_op_t window_when(const mapper_t<rx_observable_t>& closing_mapper) { return ops::window_when_(closing_mapper); }
// _windowwithcount.h
static observable_op_t window_with_count(int64_t count, int64_t skip = -1) { return ops::window_with_count_(count, skip); }
// _windowwithtime.h
static observable_op_t window_with_time(const time_delta_t& timespan, const time_delta_t& timeshift = DELTA_ZERO, const scheduler_t& scheduler = nullptr) { return ops::window_with_time_(timespan, timeshift, scheduler); }
// _windowwithtimeorcount.h
static observable_op_t window_with_time_or_count(const time_delta_t& timespan, int64_t count, const scheduler_t& scheduler = nullptr) { return ops::window_with_time_or_count_(timespan, count, scheduler); }
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