#ifndef RX_WRAPPER_WRAPPERCAST_H
#define RX_WRAPPER_WRAPPERCAST_H

#include "cast.h"
#include "exception/exception.h"

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

using namespace godot;

namespace rx::abstract {
    class DisposableBase;
    class IterableBase;
    class IteratorBase;
    class LockBase;
    class ObservableBase;
    class ObserverBase;
    class SubjectBase;
    class SchedulerBase;

}
namespace rx::wrappers {
    class RxDisposableBase;
    class RxIterableBase;
    class RxIteratorBase;
    class RxLockBase;
    class RxObservableBase;
    class RxObserverBase;
    class RxSubjectBase;
    class RxSchedulerBase;
}

using namespace rx::abstract;
using namespace rx::wrappers;

template<class FromT, class FromW, class ToT, class ToW>
static Ref<ToW> dyn_wrapper_cast_(Ref<FromW> ref) {
    std::shared_ptr<FromT> from = FromW::unwrap(ref);
    std::shared_ptr<ToT> to = std::dynamic_pointer_cast<ToT>(from);
    return ToW::wrap(to);
}
template<class ToT, class ToW>
static Ref<ToW> dyn_wrapper_cast_or_null(const Variant& source) {
    if (auto _source = DYN_CAST_OR_NULL(source, RxDisposableBase)) {
        return dyn_wrapper_cast_<DisposableBase, RxDisposableBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxIterableBase)) {
        return dyn_wrapper_cast_<IterableBase, RxIterableBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxIteratorBase)) {
        return dyn_wrapper_cast_<IteratorBase, RxIteratorBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxLockBase)) {
        return dyn_wrapper_cast_<LockBase, RxLockBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxObservableBase)) {
        return dyn_wrapper_cast_<ObservableBase, RxObservableBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxObserverBase)) {
        return dyn_wrapper_cast_<ObserverBase, RxObserverBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxSubjectBase)) {
        return dyn_wrapper_cast_<SubjectBase, RxSubjectBase, ToT, ToW>(_source);
    }
    if (auto _source = DYN_CAST_OR_NULL(source, RxSchedulerBase)) {
        return dyn_wrapper_cast_<SchedulerBase, RxSchedulerBase, ToT, ToW>(_source);
    }
    return Ref<ToW>();
}
template<class ToT, class ToW>
static Ref<ToW> dyn_wrapper_cast(const Variant& source) {
    auto res = dyn_wrapper_cast_or_null<ToT, ToW>(source);
    if (res.is_null()) {
        throw rx::exception::BadCastException();
    }
    return res;
}

#endif // RX_WRAPPER_WRAPPERCAST_H