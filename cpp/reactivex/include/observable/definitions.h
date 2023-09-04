#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "observable/case.h"
#include "observable/catch.h"
#include "observable/combinelatest.h"
#include "observable/concat.h"
#include "observable/defer.h"
#include "observable/empty.h"
#include "observable/forkjoin.h"
#include "observable/fromiterable.h"
#include "observable/generate.h"
#include "observable/ifthen.h"
#include "observable/merge.h"
#include "observable/never.h"
#include "observable/onerrorresumenext.h"
#include "observable/range.h"
#include "observable/returnvalue.h"
#include "observable/throw.h"
#include "observable/timer.h"
#include "observable/using.h"
#include "observable/withlatestfrom.h"
#include "observable/zip.h"

#include "observable/operators/_all.h"
#include "observable/operators/_amb.h"
#include "observable/operators/_asobservable.h"
#include "observable/operators/_average.h"
#include "observable/operators/_catch.h"
#include "observable/operators/_combinelatest.h"
#include "observable/operators/_concat.h"
#include "observable/operators/_contains.h"
#include "observable/operators/_count.h"
#include "observable/operators/_debounce.h"
#include "observable/operators/_defaultifempty.h"
#include "observable/operators/_dematerialize.h"
#include "observable/operators/_distinct.h"
#include "observable/operators/_distinctuntilchanged.h"
#include "observable/operators/_do.h"
#include "observable/operators/_elementatordefault.h"
#include "observable/operators/_exclusive.h"
#include "observable/operators/_expand.h"
#include "observable/operators/_filter.h"
#include "observable/operators/_finallyaction.h"
#include "observable/operators/_find.h"
#include "observable/operators/_first.h"
#include "observable/operators/_firstordefault.h"
#include "observable/operators/_forkjoin.h"
#include "observable/operators/_groupbyuntil.h"
#include "observable/operators/_groupby.h"
#include "observable/operators/_groupjoin.h"
#include "observable/operators/_ignoreelements.h"
#include "observable/operators/_isempty.h"
#include "observable/operators/_join.h"
#include "observable/operators/_last.h"
#include "observable/operators/_lastordefault.h"
#include "observable/operators/_map.h"
#include "observable/operators/_materialize.h"
#include "observable/operators/_max.h"
#include "observable/operators/_maxby.h"
#include "observable/operators/_merge.h"
#include "observable/operators/_min.h"
#include "observable/operators/_minby.h"
#include "observable/operators/_multicast.h"
#include "observable/operators/_observeon.h"
#include "observable/operators/_onerrorresumenext.h"
#include "observable/operators/_pairwise.h"
#include "observable/operators/_publish.h"
#include "observable/operators/_reduce.h"
#include "observable/operators/_scan.h"
#include "observable/operators/_some.h"
#include "observable/operators/_take.h"
#include "observable/operators/_zip.h"

#include "observable/operators/connectable/_refcount.h"

namespace rx::observable {

struct Observables {

// case.h
template<typename KeyT, typename MappingT> inline static std::shared_ptr<Observable> case_mapper(const mapper_t<KeyT>& mapper, const MappingT& sources, const std::shared_ptr<Observable>& default_source = nullptr) { return case_(mapper, sources, default_source); }
// catch.h
template<typename IterableT> inline static std::shared_ptr<Observable> catch_with_iterable(const IterableT& sources) { return catch_with_iterable(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> catch_with_variadic(const Args&... sources) { return catch_with_variadic_(sources...); }
// combinelatest.h
template<typename IterableT> inline static std::shared_ptr<Observable> combine_latest(const IterableT& sources) { return combine_latest_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> combine_latest(const Args&... sources) { return combine_latest_(sources...); }
// concat.h
template<typename IterableT> inline static std::shared_ptr<Observable> concat_with_iterable(const IterableT& sources) { return concat_with_iterable_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> concat_with_variadic(const Args&... sources) { return concat_with_variadic_(sources...); }
template<typename... Args> inline static std::shared_ptr<Observable> concat(const Args&... sources) { return concat_with_variadic_(sources...); }
// defer.h
inline static std::shared_ptr<Observable> defer(const observable_factory_t& factory) { return defer_(factory); }
// empty.h
inline static std::shared_ptr<Observable> empty(const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return empty_(scheduler); }
// forkjoin.h
template<typename IterableT> inline static std::shared_ptr<Observable> fork_join(const IterableT& sources) { return fork_join_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> fork_join(const Args&... sources) { return fork_join_(sources...); }
// fromiterable.h
template<typename IterableT> inline static std::shared_ptr<Observable> from_iterable(const IterableT& iterable, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return from_iterable_(iterable, scheduler); }
// generate.h
inline static std::shared_ptr<Observable> generate(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate) { return generate_(initial_state, condition, iterate); }
// ifthen.h
inline static std::shared_ptr<Observable> if_then(const predicate_t<>& condition, const std::shared_ptr<Observable>& then_source, const std::shared_ptr<Observable>& else_source = nullptr) { return if_then_(condition, then_source, else_source); }
// never.h
inline static std::shared_ptr<Observable> never() { return never_(); }
// onerrorresumenext.h
template<typename IterableT> inline static std::shared_ptr<Observable> on_error_resume_next(const IterableT& sources) { return on_error_resume_next_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> on_error_resume_next(const Args&... sources) { return on_error_resume_next_(sources...); }
// range.h
inline static std::shared_ptr<Observable> range(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return range_(start, stop, step, scheduler); }
// returnvalue.h
inline static std::shared_ptr<Observable> return_value(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return return_value_(value, scheduler); }
inline static std::shared_ptr<Observable> just(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return return_value_(value, scheduler); }
// throw.h
inline static std::shared_ptr<Observable> throw_error(const std::exception_ptr& exception, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return throw_(exception, scheduler); }
// timer.h
template<typename timeT> inline static std::shared_ptr<Observable> timer(const timeT& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return timer_(duetime, scheduler); }
template<typename timeT> inline static std::shared_ptr<Observable> periodic_timer(const timeT& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return timer_(duetime, period, scheduler); }
// using.h
static std::shared_ptr<Observable> using_resource(const std::function<disposable_t()>& resource_factory, const std::function<std::shared_ptr<Observable>(const disposable_t&)>& observable_factory) { return using_(resource_factory, observable_factory); }
// withlatestfrom.h
template<typename IterableT> inline static std::shared_ptr<Observable> with_latest_from(const std::shared_ptr<Observable>& parent, const IterableT& sources) { return with_latest_from_(parent, sources); }
template<typename... Args> inline static std::shared_ptr<Observable> with_latest_from(const std::shared_ptr<Observable>& parent, const Args&... sources) { return with_latest_from_(parent, sources...); }
// zip.h
template<typename IterableT> inline static std::shared_ptr<Observable> zip(const IterableT& sources) { return zip_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> zip(const Args&... sources) { return zip_(sources...); }

}; // END struct Observables

namespace op {

struct Operators {

// _all.h
inline static observable_op_t all(const predicate_t<Variant>& predicate) { return all_(predicate); }
// _amb.h
inline static observable_op_t amb(const std::shared_ptr<Observable>& right_source) { return amb_(right_source); }
// _asobservable.h
inline static observable_op_t as_observable() { return as_observable_(); }
// _average.h
inline static observable_op_t average(const mapper_t<double, Variant>& key_mapper = nullptr) { return average_(key_mapper); }
// _catch.h
inline static observable_op_t catch_with_handler(const std::shared_ptr<Observable>& handler) { return catch_(handler); }
inline static observable_op_t catch_with_handler(const handler_t& handler) { return catch_(handler); }
// _combinelatest.h
template <typename IterableT> inline static observable_op_t combine_latest(const IterableT& others) { return rx::observable::op::combine_latest_(others); }
template <typename... Args> inline static observable_op_t combine_latest(const Args&... others) { return rx::observable::op::combine_latest_(others...); }
// _concat.h
template<typename IterableT> inline static observable_op_t concat(const IterableT& sources) { return rx::observable::op::concat_(sources); }
template<typename... Args> inline static observable_op_t concat(const Args&... others) { return rx::observable::op::concat_(others...); }
// _contains.h
inline static observable_op_t contains(const Variant& value, const comparer_t<Variant>& comparer = nullptr) { return contains_(value, comparer); }
// _count.h
inline static observable_op_t count(const predicate_t<Variant>& predicate = nullptr) { return count_(predicate); }
// _debounce.h
inline static observable_op_t debounce(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) { return debounce_(duetime, scheduler); }
inline static observable_op_t throttle_with_mapper(const mapper_t<rx_observable_t, Variant>& throttle_duration_mapper) { return throttle_with_mapper_(throttle_duration_mapper); }
// _defaultifempty.h
inline static observable_op_t default_if_empty(const Variant& default_value = VNULL) { return default_if_empty_(default_value); }
// _dematerialize.h
inline static observable_op_t dematerialize() { return dematerialize_(); }
// _distinct.h
inline static observable_op_t distinct(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr) { return distinct_(key_mapper, comparer); }
// _distinctuntilchanged.h
inline static observable_op_t distinct_until_changed(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr) { return distinct_until_changed_(key_mapper, comparer); }
// _do.h
inline static observable_op_t do_action(const on_next_t& on_next = nullptr, const on_error_t& on_error = nullptr, const on_completed_t& on_completed = nullptr) { return do_action_(on_next, on_error, on_completed); }
inline static observable_op_t do_observer(const observer_t& observer) { return do_(observer); }
inline static rx_observable_t do_after_next(const rx_observable_t& source, const on_next_t& after_next) { return rx::observable::op::do_after_next(source, after_next); }
inline static rx_observable_t do_on_subscribe(const rx_observable_t& source, const do_action_t& on_subscribe) { return rx::observable::op::do_on_subscribe(source, on_subscribe); }
inline static rx_observable_t do_on_dispose(const rx_observable_t& source, const dispose_t& on_dispose) { return rx::observable::op::do_on_dispose(source, on_dispose); }
inline static rx_observable_t do_on_terminate(const rx_observable_t& source, const do_action_t& on_terminate) { return rx::observable::op::do_on_terminate(source, on_terminate); }
inline static observable_op_t do_finally(const do_action_t& finally_action) { return do_finally_(finally_action); }
// _elementatordefault.h
inline static observable_op_t element_at_or_default(uint64_t index, bool has_default = false, const Variant& default_value = VNULL) { return element_at_or_default_(index, has_default, default_value); }
// _exclusive.h
inline static observable_op_t exclusive() { return exclusive_(); }
// _expand.h
inline static observable_op_t expand(const mapper_t<rx_observable_t, Variant>& mapper) { return expand_(mapper); }
// _finallyaction.h
inline static observable_op_t finally_action(const std::function<void()>& action) { return finally_action_(action); }
// _find.h
inline static observable_op_t find_value(const predicate_t<Variant, uint64_t, rx_observable_t>& predicate, bool yield_index) { return find_value_(predicate, yield_index); }
// _first.h
inline static observable_op_t first(const predicate_t<Variant>& predicate = nullptr) { return first_(predicate); }
// _firstordefault.h
inline static observable_op_t first_or_default(const predicate_t<Variant>& predicate = nullptr, const Variant& default_value = VNULL) { return first_or_default_(predicate, default_value); }
// _forkjoin.h
template<typename IterableT> inline static observable_op_t fork_join(const IterableT& args) { return fork_join(args); }
template <typename... Args> inline static observable_op_t fork_join(const Args&... others) { return fork_join_(others...); }
// _filter.h
inline static observable_op_t filter(const predicate_t<Variant>& predicate) { return filter_(predicate); }
inline static observable_op_t filter_indexed(const predicate_indexed_t<Variant>& predicate = nullptr) { return filter_indexed_(predicate); }
// _groupby.h
inline static observable_op_t group_by(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr,const mapper_t<rx_subject_t>& subject_mapper = nullptr) { return group_by_(key_mapper, element_mapper, subject_mapper); }
// _groupbyuntil.h
inline static observable_op_t group_by_until(const mapper_t<Variant, Variant>& key_mapper, const mapper_t<rx_observable_t, grouped_observable_t>& duration_mapper, const mapper_t<Variant, Variant>& element_mapper = nullptr, const mapper_t<rx_subject_t>& subject_mapper = nullptr
) { return group_by_until_(key_mapper, duration_mapper, element_mapper, subject_mapper); }
// _groupjoin.h
inline static observable_op_t group_join(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper) { return group_join_(right, left_duration_mapper, right_duration_mapper); }
// _ignoreelements.h
inline static observable_op_t ignore_elements() { return ignore_elements_(); }
// _isempty.h
inline static observable_op_t is_empty() { return is_empty_(); }
// _join.h
inline static observable_op_t join(const rx_observable_t& right, const mapper_t<rx_observable_t, Variant>& left_duration_mapper, const mapper_t<rx_observable_t, Variant>& right_duration_mapper) { return join_(right, left_duration_mapper, right_duration_mapper); }
// _last.h
inline static observable_op_t last(const predicate_t<Variant>& predicate = nullptr) { return last_(predicate); }
// _lastordefault.h
inline static observable_op_t last_or_default(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr) { return last_or_default_(default_value, predicate); }
// _reduce.h
inline static observable_op_t reduce(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return reduce_(accumulator, seed); }
// _map.h
inline static observable_op_t map(const mapper_t<Variant, Variant>& mapper = nullptr) { return map_(mapper); }
inline static observable_op_t map_indexed(const mapper_indexed_t<Variant, Variant>& mapper = nullptr) { return map_indexed_(mapper); }
// _materalize.h
inline static observable_op_t materialize() { return materialize_(); }
// _scan.h
inline static observable_op_t scan(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return scan_(accumulator, seed); }
// _some.h
inline static observable_op_t some(const predicate_t<Variant>& predicate = nullptr) { return some_(predicate); }
// _take.h
inline static observable_op_t take(uint64_t count) { return take_(count); }
// _zip.h
template<typename IterableT> inline static observable_op_t zip(const IterableT& args) { return rx::observable::op::zip_(args); }
template <typename... Args> inline static observable_op_t zip(const Args&... others) { return rx::observable::op::zip_(others...); }
template<typename IterableT> inline static observable_op_t zip_with_iterable(const IterableT& seq) { return zip_with_iterable_(seq); }

}; // END struct Operators

} // END namespace op
} // END namespace rx::observable

#endif // RX_OBSERVABLE_DEFINITIONS_H