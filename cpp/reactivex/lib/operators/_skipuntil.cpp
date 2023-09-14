#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::skip_until_(const rx_observable_t& other) {

    auto skip_until = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto is_open = std::make_shared<bool>(false);

            on_next_t on_next = [=](const Variant& left) {
                if (*is_open) {
                    observer->on_next(left);
                }
            };

            on_completed_t on_completed = [=]() {
                if (*is_open) {
                    observer->on_completed();
                }
            };

            auto subs = source->subscribe(
                on_next, ONERROR_FWD(observer), on_completed, scheduler
            );
            auto subscriptions = std::make_shared<CompositeDisposable>();
            subscriptions->add(subs);

            auto right_subscription = std::make_shared<SingleAssignmentDisposable>();
            subscriptions->add(right_subscription);

            on_next_t on_next2 = [=](const Variant& x) {
                *is_open = true;
                right_subscription->dispose();
            };

            on_completed_t on_completed2 = [=]() {
                right_subscription->dispose();
            };

            right_subscription->set_disposable(other->subscribe(
                on_next2, ONERROR_FWD(observer), on_completed2, scheduler
            ));

            return subscriptions;
        };

        return Observable::get(subscribe);
    };

    return skip_until;
}

} // END namespace rx::observable::op
