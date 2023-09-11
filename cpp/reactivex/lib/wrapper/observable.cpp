#include "wrapper/observable.h"

#include "observable/definitions.h"
#include "internal/utils.h"

using namespace rx::observable;

namespace rx::wrappers {

// case.h
Ref<RxObservable> RxObservable::case_mapper(const Callable& mapper, const Dictionary& sources, Ref<RxObservable> default_source) { 
    return obs::case_(from_cb<Variant>(mapper), sources, default_source); }
// defer.h
Ref<RxObservable> RxObservable::defer(const Callable& factory) {
    return obs::defer_(observable_factory_cb(factory)); }
// empty.h
Ref<RxObservable> RxObservable::empty(Ref<RxSchedulerBase> scheduler) { return obs::empty_(scheduler); }
// throw.h
Ref<RxObservable> RxObservable::throw_message(const String& what, const String& type, Ref<RxSchedulerBase> scheduler) {
    try { throw rx_exception(what.ascii().get_data(), type.ascii().get_data()); } catch(...) { return obs::throw_(std::current_exception(), scheduler); }}
Ref<RxObservable> RxObservable::throw_error(Ref<RxError> error, Ref<RxSchedulerBase> scheduler) {
    return obs::throw_(RxError::unwrap(error), scheduler); }

void RxObservable::_bind_methods() {
    /* Constructors */
    ClassDB::bind_static_method("RxObservable", D_METHOD("case", "mapper", "sources", "default"), &RxObservable::case_mapper, DEFVAL(VNULL));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("combine_latest", "sources"), &RxObservable::combine_latest);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("concat", "sources"), &RxObservable::concat);
    
    ClassDB::bind_static_method("RxObservable", D_METHOD("defer", "factory"), &RxObservable::defer);

    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(VNULL));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("fork_join", "sources"), &RxObservable::fork_join);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("from_iterable", "iterable", "scheduler"), &RxObservable::from_iterable, DEFVAL(Ref<RxSchedulerBase>()));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("from", "iterable", "scheduler"), &RxObservable::from, DEFVAL(Ref<RxSchedulerBase>()));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("generate", "initial_state", "condition", "iterate"), &RxObservable::generate);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("if_then", "condition", "then_source", "else_source"), &RxObservable::if_then, DEFVAL(Ref<RxObservable>()));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("merge", "sources"), &RxObservable::merge);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("on_error_resume_next", "sources", "use_factory"), &RxObservable::on_error_resume_next, DEFVAL(false));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("range", "start", "stop", "step"), &RxObservable::range, DEFVAL(INT64_MAX), DEFVAL(1));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>()));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::just, DEFVAL(Ref<RxSchedulerBase>()));

    ClassDB::bind_static_method("RxObservable", D_METHOD("raise", "message", "type", "scheduler"), &RxObservable::throw_message, DEFVAL("RxError"), DEFVAL(VNULL));
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "error", "scheduler"), &RxObservable::throw_error, DEFVAL(VNULL));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("timer", "duetime", "period", "scheduler"), &RxObservable::timer, DEFVAL(Variant()), DEFVAL(Ref<RxSchedulerBase>()));
    // ClassDB::bind_static_method("RxObservable", D_METHOD("periodic_timer", "period", "scheduler"), &RxObservable::periodic_timer, DEFVAL(Ref<RxSchedulerBase>()));

    // ClassDB::bind_static_method("RxObservable", D_METHOD("using", "resource_factory", "observable_factory"), &RxObservable::using_resource);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("with_latest_from", "parent", "sources"), &RxObservable::with_latest_from);

    // ClassDB::bind_static_method("RxObservable", D_METHOD("zip", "sources"), &RxObservable::zip);


    RX_WRAPPER_CAST_BINDS(RxObservable)
}

} // END namespace rx::wrappers