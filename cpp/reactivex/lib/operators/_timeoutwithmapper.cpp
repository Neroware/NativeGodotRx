#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"


namespace rx::observable::op {

observable_op_t timeout_with_mapper_(
    const rx_observable_t& first_timeout, 
    const mapper_t<rx_observable_t, Variant>& timeout_duration_mapper,
    const rx_observable_t& other
) {

    auto first_timeout_ = first_timeout ? first_timeout : never();
    if (!other) throw rx_exception("Timeout!"); auto other_ = other; 

    observable_op_t timeout_with_mapper = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

            auto subscription = std::make_shared<SerialDisposable>();
            auto timer = std::make_shared<SerialDisposable>();
            auto original = std::make_shared<SingleAssignmentDisposable>();

            subscription->set_disposable(original);

            auto switched = std::make_shared<bool>(false);
            auto _id = std::make_shared<int64_t>(0);

            auto set_timer = [=](const rx_observable_t& timeout) {
                int64_t my_id = *_id;

                auto timer_wins = [=]() {
                    return *_id == my_id;
                };

                auto d = std::make_shared<SingleAssignmentDisposable>();
                timer->set_disposable(d);

                auto on_next = [=](const Variant& x) {
                    if (timer_wins()) {
                        subscription->set_disposable(other->subscribe(observer, scheduler));
                    }

                    d->dispose();
                };

                auto on_error = [=](const std::exception_ptr& e) {
                    if (timer_wins()) {
                        observer->on_error(e);
                    }
                };

                auto on_completed = [=]() {
                    if (timer_wins()) {
                        subscription->set_disposable(other->subscribe(observer));
                    }
                };

                d->set_disposable(timeout->subscribe(
                    on_next, on_error, 
                    on_completed, scheduler
                ));
            };

            set_timer(first_timeout_);

            auto observer_wins = [=]() {
                bool res = !switched;
                if (res) {
                    ++(*_id);
                }

                return res;
            };

            auto on_next = [=](const Variant& x) {
                if (observer_wins()) {
                    observer->on_next(x);
                    rx_observable_t timeout = nullptr;

                    try {
                        timeout = timeout_duration_mapper ? timeout_duration_mapper(x) : never();
                    } catch (const std::exception_ptr& e) {
                        observer->on_error(e);
                        return;
                    }

                    set_timer(timeout);
                }
            };

            auto on_error = [=](const std::exception_ptr& error) {
                if (observer_wins()) {
                    observer->on_error(error);
                }
            };

            auto on_completed = [=]() {
                if (observer_wins()) {
                    observer->on_completed();
                }
            };

            original->set_disposable(source->subscribe(
                on_next, on_error, 
                on_completed, scheduler
            ));
            return std::make_shared<CompositeDisposable>(subscription, timer);
        };

        return Observable::get(subscribe);
    };

    return timeout_with_mapper;
}

} // END namespace rx::observable::op
