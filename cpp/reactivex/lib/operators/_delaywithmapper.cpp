#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/serialdisposable.h"

namespace rx::observable::op {

observable_op_t ops::delay_with_mapper_(
    const rx_observable_t& subscription_delay,
    const mapper_t<rx_observable_t, Variant>& subscription_delay_mapper,
    const rx_observable_t& delay_duration_mapper
) {
    auto delay_with_mapper = OP(source) {

        rx_observable_t sub_delay = subscription_delay;
        mapper_t<rx_observable_t, Variant> mapper = subscription_delay_mapper;

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto delays = std::make_shared<CompositeDisposable>();
            auto at_end = std::make_shared<bool>(false);

            auto done = [=]() {
                if (*at_end && delays->size() == 0) {
                    observer->on_completed();
                }
            };

            auto subscription = std::make_shared<SerialDisposable>();

            auto start = [=]() {
                auto on_next = [=](const Variant& x) {
                    rx_observable_t delay;
                    try {
                        assert(mapper);
                        delay = mapper(x);
                    }
                    catch (...) {
                        observer->on_error(std::current_exception());
                    }

                    auto d = std::make_shared<SingleAssignmentDisposable>();
                    delays->add(d);

                    on_next_t on_next = [=](const Variant& x) {
                        observer->on_next(x);
                        delays->remove(d);
                        done();
                    };

                    on_completed_t on_completed = [=]() {
                        observer->on_next(x);
                        delays->remove(d);
                        done();
                    };

                    d->set_disposable(delay->subscribe(
                        on_next, DEFAULT_ON_ERROR, 
                        on_completed, scheduler_
                    ));
                };

                auto on_completed = [=]() {
                    *at_end = true;
                    subscription->dispose();
                    done();
                };

                subscription->set_disposable(source->subscribe(
                    on_next, ONERROR_FWD(observer), 
                    on_completed, scheduler_
                ));
            };

            if (!sub_delay) {
                start();
            } else {
                subscription->set_disposable(subscription_delay->subscribe(
                    [=](const Variant&) { start(); }, 
                    ONERROR_FWD(observer), start
                ));
            }

            return std::make_shared<CompositeDisposable>(subscription, delays);
        };

        return Observable::get(subscribe);
    };

    return delay_with_mapper;
}

} // END namespace rx::observable::op