#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "observable/empty.h"
#include "observable/returnvalue.h"
#include "observable/throw.h"
#include "observable/catch.h"
#include "observable/defer.h"
#include "observable/never.h"
#include "observable/case.h"
#include "observable/combinelatest.h"
#include "observable/concat.h"
#include "observable/forkjoin.h"
#include "observable/fromiterable.h"
#include "observable/generate.h"
#include "observable/ifthen.h"
#include "observable/onerrorresumenext.h"
#include "observable/range.h"
#include "observable/timer.h"
#include "observable/using.h"
#include "observable/withlatestfrom.h"
#include "observable/zip.h"

#include "observable/operators/_filter.h"
#include "observable/operators/_amb.h"
#include "observable/operators/_asobservable.h"
#include "observable/operators/_map.h"
#include "observable/operators/_lastordefault.h"
#include "observable/operators/_last.h"
#include "observable/operators/_scan.h"
#include "observable/operators/_average.h"

namespace rx::observable {

struct Observables {

inline static std::shared_ptr<Observable> empty_(const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::empty_(scheduler);
}
inline static std::shared_ptr<Observable> return_value_(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler_ = nullptr) {
    return rx::observable::return_value_(value, scheduler_);
}
inline static std::shared_ptr<Observable> never_() {
    return rx::observable::never_();
}
inline static std::shared_ptr<Observable> throw_(const std::exception_ptr& exception, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::throw_(exception, scheduler);
}
template<typename T>
inline static std::shared_ptr<Observable> catch_with_iterable_(const T& sources) {
    return rx::observable::catch_with_iterable_(sources);
}
inline static std::shared_ptr<Observable> defer_(const observable_factory_t& factory) {
    return rx::observable::defer_(factory);
}
template<typename KeyT, typename MappingT>
inline static std::shared_ptr<Observable> case_(const std::function<KeyT()>& mapper, const MappingT& sources, const std::shared_ptr<Observable>& default_source = nullptr) {
    return rx::observable::case_(mapper, sources, default_source);
}
template<typename T>
inline static std::shared_ptr<Observable> combine_latest_(const T& sources) {
    return rx::observable::combine_latest_(sources);
}
template<typename T>
inline static std::shared_ptr<Observable> concat_with_iterable_(const T& sources) {
    return rx::observable::concat_with_iterable_(sources);
}
template<typename T>
inline static std::shared_ptr<Observable> fork_join_(const T& sources) {
    return rx::observable::fork_join_(sources);
}
inline static std::shared_ptr<Observable> from_iterable_(const std::shared_ptr<IterableBase>& iterable, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::from_iterable_(iterable, scheduler);
}
inline static std::shared_ptr<Observable> generate_(
    const Variant& initial_state, 
    const predicate_t<Variant>& condition,
    const mapper_t<Variant, Variant>& iterate
) {
    return rx::observable::generate_(initial_state, condition, iterate);
}
inline static std::shared_ptr<Observable> if_then_(const predicate_t<>& condition, const std::shared_ptr<Observable>& then_source, const std::shared_ptr<Observable>& else_source = nullptr) {
    return rx::observable::if_then_(condition, then_source, else_source);
}
template<typename T>
inline static std::shared_ptr<Observable> on_error_resume_next_(const T& sources) {
    return rx::observable::on_error_resume_next_(sources);
}
inline static std::shared_ptr<Observable> range_(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::range_(start, stop, step);
}
template<typename timeT>
inline static std::shared_ptr<Observable> timer_(const timeT& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::timer_(duetime, scheduler);
}
template<typename timeT>
inline static std::shared_ptr<Observable> periodic_timer_(const timeT& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::timer_(duetime, period, scheduler);
}
inline static std::shared_ptr<Observable> using_(
    const std::function<disposable_t()>& resource_factory,
    const std::function<std::shared_ptr<Observable>(const disposable_t&)>& observable_factory
) {
    return rx::observable::using_(resource_factory, observable_factory);
}
template<typename T>
inline static std::shared_ptr<Observable> with_latest_from_(
    const std::shared_ptr<Observable>& parent,
    const T& sources
) {
    return rx::observable::with_latest_from_(parent, sources);
}
template<typename T>
inline static std::shared_ptr<Observable> zip_(const T& sources) {
    return rx::observable::zip_(sources);
}

}; // END struct Observables

struct Operators {

inline static observable_op_t filter_(const predicate_t<Variant>& predicate) {
    return rx::observable::filter_(predicate);
}
inline static observable_op_t filter_indexed_(const predicate_indexed_t<Variant>& predicate = nullptr) {
    return rx::observable::filter_indexed_(predicate);
}
inline static observable_op_t amb_(const std::shared_ptr<Observable>& right_source) {
    return rx::observable::amb_(right_source);
}
inline static observable_op_t as_observable_() {
    return rx::observable::as_observable_();
}
inline static observable_op_t map_(const mapper_t<Variant, Variant>& mapper = nullptr) {
    return rx::observable::map_(mapper);
}
// TODO map_indexed
inline static observable_op_t last_or_default_(const Variant& default_value = Variant(), const predicate_t<Variant>& predicate = nullptr) {
    return rx::observable::last_or_default_(default_value, predicate);
}
inline static observable_op_t last_(const predicate_t<Variant>& predicate = nullptr) {
    return rx::observable::last_(predicate);
}
inline static observable_op_t scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) {
    return rx::observable::scan_(accumulator, seed);
}
inline static observable_op_t average_(const mapper_t<double, Variant>& key_mapper = nullptr) {
    return rx::observable::average_(key_mapper);
}

}; // END struct Operators

} // END namespace rx::observable

#define OBSERVABLE_CONSTRUCTORS_BINDS \
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("raise", "message", "type", "scheduler"), &RxObservable::throw_message, DEFVAL("RxError"), DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "error", "scheduler"), &RxObservable::throw_error, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("defer", "factory"), &RxObservable::defer); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("case", "mapper", "sources", "default"), &RxObservable::case_mapper, DEFVAL(Ref<RxObservable>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("combine_latest", "sources"), &RxObservable::combine_latest); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("concat", "sources"), &RxObservable::concat_with_iterable); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("fork_join", "sources"), &RxObservable::fork_join); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("from_iterable", "iterable", "scheduler"), &RxObservable::from_iterable, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("from", "iterable", "scheduler"), &RxObservable::from_iterable, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("generate", "initial_state", "condition", "iterate"), &RxObservable::generate); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("if_then", "condition", "then_source", "else_source"), &RxObservable::if_then, DEFVAL(Ref<RxObservable>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("on_error_resume_next", "sources", "use_factory"), &RxObservable::on_error_resume_next, DEFVAL(false)); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("range", "start", "stop", "step"), &RxObservable::range, DEFVAL(INT64_MAX), DEFVAL(1)); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("timer", "duetime", "period", "scheduler"), &RxObservable::timer, DEFVAL(Variant()), DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("periodic_timer", "period", "scheduler"), &RxObservable::periodic_timer, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("using", "resource_factory", "observable_factory"), &RxObservable::using_resource); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("with_latest_from", "parent", "sources"), &RxObservable::with_latest_from); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("zip", "sources"), &RxObservable::zip); \

#define OBSERVABLE_OPERATOR_BINDS \
    ClassDB::bind_method(D_METHOD("filter", "predicate"), &RxObservable::filter); \
    ClassDB::bind_method(D_METHOD("filter_indexed", "predicate"), &RxObservable::filter_indexed, DEFVAL(Callable())); \
    ClassDB::bind_method(D_METHOD("amb", "right_source"), &RxObservable::amb); \
    ClassDB::bind_method(D_METHOD("as_observable"), &RxObservable::as_observable); \
    ClassDB::bind_method(D_METHOD("map", "mapper"), &RxObservable::map, DEFVAL(Callable())); \
    ClassDB::bind_method(D_METHOD("last_or_default", "default_value", "predicate"), &RxObservable::last_or_default, DEFVAL(Variant()), DEFVAL(Callable())); \
    ClassDB::bind_method(D_METHOD("last", "predicate"), &RxObservable::last, DEFVAL(Callable())); \
    ClassDB::bind_method(D_METHOD("scan", "accumulator", "seed"), &RxObservable::scan, DEFVAL(memnew(NotSet))); \
    ClassDB::bind_method(D_METHOD("average", "key_mapper"), &RxObservable::average, DEFVAL(Callable())); \

#endif // RX_OBSERVABLE_DEFINITIONS_H