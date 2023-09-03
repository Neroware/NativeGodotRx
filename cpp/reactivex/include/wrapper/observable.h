#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/abstract.h"
#include "wrapper/disposable.h"
#include "observable/definitions.h"

#include "observable/connectableobservable.h"
#include "observable/groupedobservable.h"

#include "basic.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::observable;

namespace rx {

static subscription_t subscription_cb(const Callable& cb);
static observable_factory_t observable_factory_cb(const Callable& cb);

namespace wrappers {

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

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxConnectableObservable)
        ClassDB::bind_static_method("RxConnectableObservable", D_METHOD("get", "source", "subject"), &RxConnectableObservable::get);
        ClassDB::bind_method(D_METHOD("connected", "scheduler"), &RxConnectableObservable::connect, DEFVAL(Ref<RxSchedulerBase>()));
        ClassDB::bind_method(D_METHOD("auto_connect", "subscriber_count"), &RxConnectableObservable::auto_connect, DEFVAL(1));
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

using namespace rx::wrappers;

observable_factory_t observable_factory_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](const std::shared_ptr<SchedulerBase>& scheduler) -> std::shared_ptr<Observable> {
        return RxObservable::unwrap(cb.callv(Array::make(RxSchedulerBase::wrap(scheduler))));
    };
}

} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H