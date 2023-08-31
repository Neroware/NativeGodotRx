#ifndef RX_OBSERVABLE_OPERATORS_SOME_H
#define RX_OBSERVABLE_OPERATORS_SOME_H

#include "observable/observable.h"
#include "observable/operators/_filter.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t some_(const predicate_t<Variant>& predicate = nullptr) {

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

#endif // RX_OBSERVABLE_OPERATORS_SOME_H
