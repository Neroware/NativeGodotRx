#include "observable/definitions.h"

using namespace rx::disposable;

namespace rx::observable::op {

observable_op_t ops::as_observable_() {

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