#include "observable/definitions.h"

#include "disposable/compositedisposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"

namespace rx::observable::op {

observable_op_t ops::switch_latest_() {

    observable_op_t switch_latest = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto inner_subscription = std::make_shared<SerialDisposable>();
            auto has_latest = std::make_shared<bool>(false);
            auto is_stopped = std::make_shared<bool>(false);
            auto latest = std::make_shared<int>(0);

            auto on_next = [=](const Variant& inner_source) {
                auto d = std::make_shared<SingleAssignmentDisposable>();
                int _id;
                {
                    std::lock_guard<RLock> guard(source->lock);
                    (*latest)++;
                    _id = *latest;
                }
                *has_latest = true;
                inner_subscription->set_disposable(d);

                rx_observable_t obs = inner_source;

                auto on_next_inner = [=](const Variant& x) {
                    if (*latest == _id) {
                        observer->on_next(x);
                    }
                };

                auto on_error_inner = [=](const std::exception_ptr& e) {
                    if (*latest == _id) {
                        observer->on_error(e);
                    }
                };

                auto on_completed_inner = [=]() {
                    if (*latest == _id) {
                        *has_latest = false;
                        if (*is_stopped) {
                            observer->on_completed();
                        }
                    }
                };

                d->set_disposable(obs->subscribe(
                    on_next_inner, on_error_inner, 
                    on_completed_inner, scheduler
                ));
            };

            auto on_completed = [=]() {
                *is_stopped = true;
                if (!*has_latest) {
                    observer->on_completed();
                }
            };

            auto subscription = source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );

            return std::make_shared<CompositeDisposable>(subscription, inner_subscription);
        };

        return Observable::get(subscribe);
    };

    return switch_latest;
}

} // END namespace rx::observable::op
