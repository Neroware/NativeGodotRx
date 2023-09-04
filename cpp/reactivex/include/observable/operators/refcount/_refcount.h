#ifndef RX_OBSERVABLE_OPERATORS_REFCOUNT_H
#define RX_OBSERVABLE_OPERATORS_REFCOUNT_H

#include "observable/observable.h"
#include "observable/connectableobservable.h"

namespace rx::observable::op {

typedef std::shared_ptr<rx::observable::ConnectableObservable> connectable_observable_t;
typedef std::function<rx_observable_t(const connectable_observable_t&)> connectable_observable_op_t;

static connectable_observable_op_t ref_count_() {

    connectable_observable_op_t ref_count = [=](const connectable_observable_t& source) -> rx_observable_t {

        auto connectable_subscription = std::make_shared<disposable_t>(nullptr);
        auto count = std::make_shared<uint64_t>(0);

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            ++(*count);
            bool should_connect = *count == 1;
            auto subscription = source->subscribe(observer, scheduler);

            if (should_connect) {
                *connectable_subscription = source->connect(scheduler);
            }

            dispose_t dispose = [=]() {
                subscription->dispose();
                --(*count);
                if (*count == 0 && *connectable_subscription) {
                    (*connectable_subscription)->dispose();
                }
            };

            return std::make_shared<Disposable>(dispose);
        };

        return Observable::get(subscribe);
    };

    return ref_count;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_REFCOUNT_H
