#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/abstract.h"
#include "observable/definitions.h"

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
    static inline void _bind_methods() {
        OBSERVABLE_CONSTRUCTORS_BINDS
        RX_WRAPPER_CAST_BINDS(RxObservable)
    }
};

} // END namespace wrapper

using namespace rx::wrappers;

observable_factory_t observable_factory_cb(const Callable& cb) {
    return [cb](const std::shared_ptr<SchedulerBase>& scheduler) -> std::shared_ptr<Observable> {
        return RxObservable::unwrap(cb.callv(Array::make(RxSchedulerBase::wrap(scheduler))));
    };
}

} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H