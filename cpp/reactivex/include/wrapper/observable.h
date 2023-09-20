#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/abstract.h"
#include "wrapper/disposable.h"
#include "wrapper/subject.h"

#include "observable/connectableobservable.h"
#include "observable/groupedobservable.h"

#include "basic.h"

#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/http_request.hpp>

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::observable;

namespace rx {
namespace wrappers {

class RxConnectableObservable;
class RxGroupedObservable;

class RxObservable : public RxObservableBase {
    GDCLASS(RxObservable, RxObservableBase)
    RX_WRAPPER(RxObservable, Observable, RxObservableBase, ObservableBase)

public:
#include "observable/definitions.inc"

protected:
    static void _bind_methods();
};

class RxConnectableObservable : public RxObservable {
    GDCLASS(RxConnectableObservable, RxObservable)
    RX_WRAPPER(RxConnectableObservable, ConnectableObservable, RxObservable, Observable)

public:
    inline static Ref<RxConnectableObservable> get(Ref<RxObservableBase> source, Ref<RxSubjectBase> subject) {
        return RxConnectableObservable::wrap(ConnectableObservable::get(
            RxObservableBase::unwrap(source),
            RxSubjectBase::unwrap(source)
        ));
    }
    inline Ref<RxDisposableBase> connect(Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
        return RxDisposableBase::wrap(this->_ptr->connect(
            RxSchedulerBase::unwrap(scheduler)
        ));
    }
    inline Ref<RxObservable> auto_connect(uint64_t subscriber_count = 1) {
        return RxObservable::wrap(this->_ptr->auto_connect(subscriber_count));
    }
    // connectable/_refcount.h
    // Ref<RxObservable> ref_count();

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxConnectableObservable)
        ClassDB::bind_static_method("RxConnectableObservable", D_METHOD("get", "source", "subject"), &RxConnectableObservable::get);
        ClassDB::bind_method(D_METHOD("connected", "scheduler"), &RxConnectableObservable::connect, DEFVAL(Ref<RxSchedulerBase>()));
        ClassDB::bind_method(D_METHOD("auto_connect", "subscriber_count"), &RxConnectableObservable::auto_connect, DEFVAL(1));
        // ClassDB::bind_method(D_METHOD("ref_count"), &RxConnectableObservable::ref_count);
    }
};


class RxGroupedObservable : public RxObservable {
    GDCLASS(RxGroupedObservable, RxObservable)
    RX_WRAPPER(RxGroupedObservable, GroupedObservable, RxObservable, Observable)

public:
    inline static Ref<RxGroupedObservable> get(const Variant& key, Ref<RxObservable> underlying_observable, Ref<RxRefCountDisposable> merged_disposable = Ref<RxRefCountDisposable>()) {
        return RxGroupedObservable::wrap(GroupedObservable::get(
            key,
            RxObservable::unwrap(underlying_observable),
            RxRefCountDisposable::unwrap(merged_disposable)
        ));
    }

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxGroupedObservable)
        ClassDB::bind_static_method("RxGroupedObservable", D_METHOD("get", "key", "underlying_observable", "merged_disposable"), &RxGroupedObservable::get, DEFVAL(Ref<RxRefCountDisposable>()));
    }
};


} // END namespace wrapper
} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H