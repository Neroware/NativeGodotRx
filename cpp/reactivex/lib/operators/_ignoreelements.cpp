#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::ignore_elements_() {

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
