#ifndef RX_OBSERVABLE_OPERATORS_PAIRWISE_H
#define RX_OBSERVABLE_OPERATORS_PAIRWISE_H

#include "observable/observable.h"
#include "observer/observer.h"
#include "typing.h"

namespace rx::observable::op {

static observable_op_t pairwise_() {

    observable_op_t pairwise = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto has_previous = std::make_shared<bool>(false);
            auto previous = std::make_shared<Variant>();

            on_next_t on_next = [=](const Variant& x) {
                Variant pair;

                {
                    std::lock_guard<RLock> guard(source->lock);
                    if (*has_previous) {
                        pair = Array::make(*previous, x);
                    } else {
                        *has_previous = true;
                    }

                    *previous = x;
                }

                if (pair) {
                    observer->on_next(pair);
                }
            };

            return source->subscribe(on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler);
        };

        return Observable::get(subscribe);
    };

    return pairwise;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_PAIRWISE_H
