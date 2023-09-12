#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::default_if_empty_(const Variant& default_value) {

    observable_op_t default_if_empty = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

            auto found = std::make_shared<bool>(false);

            on_next_t on_next = [=](const Variant& x) {
                *found = true;
                observer->on_next(x);
            };

            on_completed_t on_completed = [=]() {
                if (!*found) {
                    observer->on_next(default_value);
                }
                observer->on_completed();
            };

            auto subscription = source->subscribe(
                on_next, ONERROR_FWD(observer), on_completed, scheduler_
            );
            return subscription;
        };

        return Observable::get(subscribe);
    };

    return default_if_empty;
}

} // END namespace rx::observable::op
