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
    const predicate_t<const Variant&>& condition,
    const mapper_t<Variant, const Variant&>& iterate
) {
    return rx::observable::generate_(initial_state, condition, iterate);
}
inline static std::shared_ptr<Observable> if_then_(const predicate_t<>& condition, const std::shared_ptr<Observable>& then_source, const std::shared_ptr<Observable>& else_source = nullptr) {
    return rx::observable::if_then_(condition, then_source, else_source);
}
template<typename T>
static std::shared_ptr<Observable> on_error_resume_next_(const T& sources) {
    return rx::observable::on_error_resume_next_(sources);
}
static std::shared_ptr<Observable> range_(int64_t start, int64_t stop = INT64_MAX, int64_t step = 1, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return rx::observable::range_(start, stop, step);
}

}; // END struct Observables

} // END namespace rx::observable

#define OBSERVABLE_CONSTRUCTORS_BINDS \
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("raise", "message", "scheduler"), &RxObservable::throw_message, DEFVAL(Ref<RxSchedulerBase>())); \
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


#endif // RX_OBSERVABLE_DEFINITIONS_H