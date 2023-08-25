#ifndef RX_OBSERVABLE_WITHLATESTFROM_H
#define RX_OBSERVABLE_WITHLATESTFROM_H

#include "observable/observable.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"
#include "internal/utils.h"

using namespace rx::disposable;

namespace rx::observable {

template<typename T>
static std::shared_ptr<Observable> with_latest_from_(
    const std::shared_ptr<Observable>& parent,
    const T& sources
) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

        auto subscribeall = [=](
            const std::shared_ptr<Observable>& parent,
            const T& children
        ) -> std::vector<std::shared_ptr<SingleAssignmentDisposable>> {

            auto n = children.size();
            std::shared_ptr<Variant[]> values(new Variant[n]());
            std::shared_ptr<bool[]> has_value(new bool[n]{ 0 });

            auto subscribechild = [=](int i, const std::shared_ptr<Observable>& child) -> std::shared_ptr<SingleAssignmentDisposable> {

                auto subscription = std::make_shared<SingleAssignmentDisposable>();

                on_next_t on_next = [=](const Variant& value) {
                    std::lock_guard<RLock> guard(parent->lock);
                    values[i] = value; has_value[i] = true;
                };

                subscription->set_disposable(child->subscribe(
                    on_next, ONERROR_FWD(observer), DEFAULT_ON_COMPLETED, scheduler
                ));
                return subscription;
            };

            auto parent_subscription = std::make_shared<SingleAssignmentDisposable>();

            on_next_t on_next = [=](const Variant& value) {
                std::lock_guard<RLock> guard(parent->lock);
                if (all(has_value, n)) {
                    auto result = Array::make(value) + tuple(values, n);
                    observer->on_next(result);
                }
            };

            auto children_subscription = std::vector<std::shared_ptr<SingleAssignmentDisposable>>(n + 1);
            for (int child_id = 0; child_id < n; child_id++) {
                children_subscription[child_id + 1] = subscribechild(child_id, children[child_id]);
            }
            auto disp = parent->subscribe(
                on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler
            );
            parent_subscription->set_disposable(disp);

            children_subscription[0] = parent_subscription;
            return children_subscription;
        };

        return std::make_shared<CompositeDisposable>(subscribeall(parent, sources));

    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_WITHLATESTFROM_H