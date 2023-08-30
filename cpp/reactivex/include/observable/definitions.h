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
#include "observable/never.h"
#include "observable/onerrorresumenext.h"
#include "observable/range.h"
#include "observable/returnvalue.h"
#include "observable/throw.h"
#include "observable/timer.h"
#include "observable/using.h"
#include "observable/withlatestfrom.h"
#include "observable/zip.h"

#include "observable/operators/_amb.h"
#include "observable/operators/_asobservable.h"
#include "observable/operators/_average.h"
#include "observable/operators/_catch.h"
#include "observable/operators/_combinelatest.h"
#include "observable/operators/_filter.h"
#include "observable/operators/_last.h"
#include "observable/operators/_lastordefault.h"
#include "observable/operators/_map.h"
#include "observable/operators/_scan.h"

namespace rx::observable {

struct Observables {

// case.h
template<typename KeyT, typename MappingT> inline static std::shared_ptr<Observable> case_mapper(const mapper_t<KeyT>& mapper, const MappingT& sources, const std::shared_ptr<Observable>& default_source = nullptr) { return case_(mapper, sources, default_source); }
// catch.h
template<typename T> inline static std::shared_ptr<Observable> catch_with_iterable(const T& sources) { return catch_with_iterable(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> catch_with_variadic(const Args&... sources) { return catch_with_variadic_(sources...); }
// combinelatest.h
template<typename T> inline static std::shared_ptr<Observable> combine_latest(const T& sources) { return combine_latest_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> combine_latest(const Args&... sources) { return combine_latest_(sources...); }
// concat.h
template<typename T> inline static std::shared_ptr<Observable> concat_with_iterable(const T& sources) { return concat_with_iterable_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> concat_with_variadic(const Args&... sources) { return concat_with_variadic_(sources...); }
template<typename... Args> inline static std::shared_ptr<Observable> concat(const Args&... sources) { return concat_with_variadic_(sources...); }
// defer.h
inline static std::shared_ptr<Observable> defer(const observable_factory_t& factory) { return defer_(factory); }
// empty.h
inline static std::shared_ptr<Observable> empty(const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return empty_(scheduler); }
// forkjoin.h
template<typename T> inline static std::shared_ptr<Observable> fork_join(const T& sources) { return fork_join_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> fork_join(const Args&... sources) { return fork_join_(sources...); }
// fromiterable.h
inline static std::shared_ptr<Observable> from_iterable(const std::shared_ptr<IterableBase>& iterable, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) { return from_iterable_(iterable, scheduler); }
// generate.h
inline static std::shared_ptr<Observable> generate(const Variant& initial_state, const predicate_t<Variant>& condition, const mapper_t<Variant, Variant>& iterate) { return generate_(initial_state, condition, iterate); }
// ifthen.h
inline static std::shared_ptr<Observable> if_then(const predicate_t<>& condition, const std::shared_ptr<Observable>& then_source, const std::shared_ptr<Observable>& else_source = nullptr) { return if_then_(condition, then_source, else_source); }
// never.h
inline static std::shared_ptr<Observable> never() { return never_(); }
// onerrorresumenext.h
template<typename T> inline static std::shared_ptr<Observable> on_error_resume_next(const T& sources) { return on_error_resume_next_(sources); }
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
template<typename T> inline static std::shared_ptr<Observable> with_latest_from(const std::shared_ptr<Observable>& parent, const T& sources) { return with_latest_from_(parent, sources); }
template<typename... Args> inline static std::shared_ptr<Observable> with_latest_from(const std::shared_ptr<Observable>& parent, const Args&... sources) { return with_latest_from_(parent, sources...); }
// zip.h
template<typename T> inline static std::shared_ptr<Observable> zip(const T& sources) { return zip_(sources); }
template<typename... Args> inline static std::shared_ptr<Observable> zip(const Args&... sources) { return zip_(sources...); }

}; // END struct Observables

namespace op {

struct Operators {

// _amb.h
inline static observable_op_t amb(const std::shared_ptr<Observable>& right_source) { return amb_(right_source); }
// _asobservable.h
inline static observable_op_t as_observable() { return as_observable_(); }
// _average.h
inline static observable_op_t average(const mapper_t<double, Variant>& key_mapper = nullptr) { return average_(key_mapper); }
// _catch.h
inline static observable_op_t catch_with_handler(const std::shared_ptr<Observable>& handler) { return catch_(handler); }
inline static observable_op_t catch_with_handler(const handler_t& handler) { return catch_(handler); }
// filter.h
inline static observable_op_t filter(const predicate_t<Variant>& predicate) { return filter_(predicate); }
inline static observable_op_t filter_indexed(const predicate_indexed_t<Variant>& predicate = nullptr) { return filter_indexed_(predicate); }
// last.h
inline static observable_op_t last(const predicate_t<Variant>& predicate = nullptr) { return last_(predicate); }
// lastordefault.h
inline static observable_op_t last_or_default(const Variant& default_value = VNULL, const predicate_t<Variant>& predicate = nullptr) { return last_or_default_(default_value, predicate); }
// map.h
inline static observable_op_t map(const mapper_t<Variant, Variant>& mapper = nullptr) { return map_(mapper); }
// scan.h
inline static observable_op_t scan(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) { return scan_(accumulator, seed); }

}; // END struct Operators

} // END namespace op
} // END namespace rx::observable

#endif // RX_OBSERVABLE_DEFINITIONS_H