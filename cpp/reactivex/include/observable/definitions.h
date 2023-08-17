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
static std::shared_ptr<Observable> combine_latest_(const T& sources) {
    return rx::observable::combine_latest_(sources);
}

}; // END struct Observables

} // END namespace rx::observable

#define OBSERVABLE_CONSTRUCTORS_BINDS \
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("return_value", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("just", "value", "scheduler"), &RxObservable::return_value, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("never"), &RxObservable::never); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("throw", "message", "scheduler"), &RxObservable::throw_error, DEFVAL(Ref<RxSchedulerBase>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("catch", "sources"), &RxObservable::catch_with_iterable); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("defer", "factory"), &RxObservable::defer); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("case", "mapper", "sources", "default"), &RxObservable::case_mapper, DEFVAL(Ref<RxObservable>())); \
    ClassDB::bind_static_method("RxObservable", D_METHOD("combine_latest", "sources"), &RxObservable::combine_latest); \

#endif // RX_OBSERVABLE_DEFINITIONS_H