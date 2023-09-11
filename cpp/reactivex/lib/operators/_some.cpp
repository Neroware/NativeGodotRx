#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::some_(const predicate_t<Variant>& predicate) {

    observable_op_t some = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto on_next = [=](const Variant& value) {
                observer->on_next(true);
                observer->on_completed();
            };

            auto on_error = [=]() {
                observer->on_next(false);
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), on_error, scheduler
            );
        };

        if (predicate) {
            return source->pipe(
                filter_(predicate),
                some_()
            );
        }

        return Observable::get(subscribe);
    };

    return some;
}

} // END namespace rx::observable::op
