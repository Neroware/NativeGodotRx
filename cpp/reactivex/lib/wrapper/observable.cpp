#include "wrapper/observable.h"

#include "observable/definitions.h"

using namespace rx::observable;

namespace rx::wrappers {

// empty.h
Ref<RxObservable> RxObservable::empty(Ref<RxSchedulerBase> scheduler) { return obs::empty_(scheduler); }

void RxObservable::_bind_methods() {
    // empty.h
    ClassDB::bind_static_method("RxObservable", D_METHOD("empty", "scheduler"), &RxObservable::empty, DEFVAL(VNULL));

    RX_WRAPPER_CAST_BINDS(RxObservable)
}

} // END namespace rx::wrappers