#ifndef RX_OBSERVABLE_OPERATORS_TAKE_H
#define RX_OBSERVABLE_OPERATORS_TAKE_H

#include "observable/observable.h"
#include "observable/empty.h"
#include "typing.h"

namespace rx::observable::op {

static observable_op_t take_(uint64_t count) {
    if (count < 0) {
        throw ArgumentOutOfRangeException();
    }

    observable_op_t take = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto remaining = std::make_shared<uint64_t>(count);

            on_next_t on_next = [=](const Variant& value) {
                if (remaining > 0) {
                    --(*remaining);
                    observer->on_next(value);
                    if (*remaining == 0) {
                        observer->on_completed();
                    }
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return take;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_TAKE_H