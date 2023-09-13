#include "observable/definitions.h"
#include "observable/connectableobservable.h"

#include "disposable/compositedisposable.h"
#include "disposable/refcountdisposable.h"

namespace rx::observable {

rx_observable_t connectable::add_ref(const rx_observable_t& xs, const rx_rc_disposable_t& r) {
    subscription_t subscribe = SUBSCRIBE(observer, __ = nullptr) {
        return std::make_shared<CompositeDisposable>(
            r->get_disposable(),
            xs->subscribe(observer)
        );
    };

    return Observable::get(subscribe);
}

connectable_op_t connectable::ref_count_() {

    connectable_op_t ref_count = [=](const rx_connectable_observable_t& source) -> rx_observable_t {
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

} // END namespace rx::observable