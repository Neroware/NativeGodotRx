#ifndef RX_OBSERVABLE_OPERATORS_IGNOREELEMENTS_H
#define RX_OBSERVABLE_OPERATORS_IGNOREELEMENTS_H

#include "observable/observable.h"

namespace rx::observable::op {

static observable_op_t ignore_elements_() {

    observable_op_t ignore_elements = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            return source->subscribe(
                basic::noop<const Variant&>, ONERROR_FWD(observer),
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return ignore_elements;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_IGNOREELEMENTS_H
