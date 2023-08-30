#ifndef RX_OBSERVABLE_OPERATORS_ASOBSERVABLE_H
#define RX_OBSERVABLE_OPERATORS_ASOBSERVABLE_H

#include "observable/observable.h"
#include "disposable/singleassignmentdisposable.h"

using namespace rx::disposable;

namespace rx::observable::op {

static observable_op_t as_observable_() {

    observable_op_t as_observable = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto disposable = source->subscribe(observer, scheduler);
            return disposable;
        };

        return Observable::get(subscribe);
    };

    return as_observable;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ASOBSERVABLE_H
