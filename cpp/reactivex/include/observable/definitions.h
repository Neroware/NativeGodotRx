#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "observable/empty.h"
#include "observable/returnvalue.h"
#include "observable/throw.h"
#include "observable/catch.h"

namespace rx::observable {

struct Observables {

inline static std::shared_ptr<Observable> empty(const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return empty_(scheduler);
}
inline static std::shared_ptr<Observable> return_value(const Variant& value, const std::shared_ptr<SchedulerBase>& scheduler_ = nullptr) {
    return return_value_(value, scheduler_);
}
inline static std::shared_ptr<Observable> throw_error(const std::exception_ptr& exception, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return throw_(exception, scheduler);
}
template<typename T>
inline static std::shared_ptr<Observable> catch_with_iterable(const T& sources) {
    return catch_with_iterable_(sources);
}

}; // END struct Observables

} // END namespace rx::observable

#define OBSERVABLE_CONSTRUCTORS_WRAPPERS \
    inline static Ref<RxObservable> empty(Ref<RxScheduler> scheduler = Ref<RxScheduler>()) { return RxObservable::wrap(Observables::empty(RxScheduler::unwrap(scheduler))); } \
    inline static Ref<RxObservable> return_value(const Variant& value, Ref<RxScheduler> scheduler = Ref<RxScheduler>()) { return RxObservable::wrap(Observables::return_value(value, RxScheduler::unwrap(scheduler))); } \
    inline static Ref<RxObservable> throw_error(const String& what, Ref<RxScheduler> scheduler = Ref<RxScheduler>()) { try { throw rx_exception(what.ascii().get_data()); } catch(...) { return RxObservable::wrap(Observables::throw_error(std::current_exception(), RxScheduler::unwrap(scheduler))); } } \
    inline static Ref<RxObservable> catch_with_iterable(const Variant& sources) { return RxObservable::wrap(Observables::catch_with_iterable(rx_iterable<RxObservable, ObservableBase>(RxIterable::unwrap(rx::iterator::to_iterable(sources))))); }

#define OBSERVABLE_CONSTRUCTORS_BINDS \
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "message", "scheduler"), &RxObservable::throw_error, DEFVAL(Ref<RxScheduler>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable);

#endif // RX_OBSERVABLE_DEFINITIONS_H