#include "wrapper/observable.h"

#include "observable/definitions.h"
#include "internal/utils.h"

using namespace rx::observable;

#define RX_ITERABLE_T(variant) rx::iterator::to_iterable(variant)

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
}

} // END namespace rx::wrappers