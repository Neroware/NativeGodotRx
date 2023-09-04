#ifndef RX_OBSERVABLE_OPERATORS_REFCOUNT_H
#define RX_OBSERVABLE_OPERATORS_REFCOUNT_H

#include "observable/observable.h"
#include "observable/connectableobservable.h"

#include "internal/utils.h"

namespace rx::observable::op {

static observable_op_t ref_count_() {

    observable_op_t ref_count = [=](const rx_observable_t& source_) -> rx_observable_t {
        auto source = std::dynamic_pointer_cast<ConnectableObservable>(source_);
        if (!source){ 
            throw BadArgumentException("ref-count operator expected ConnectableObservable"); 
        }

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
