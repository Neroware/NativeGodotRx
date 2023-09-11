#include "observable/definitions.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

#include <cassert>

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::combine_latest_(const rx_observable_list_t& sources_) {
    observable_vec_t sources(sources_.begin(), sources_.end());
    rx_observable_t parent = sources[0];

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        
        int n = sources.size();
        std::shared_ptr<bool[]> has_value(new bool[n]{ 0 });
        auto has_value_all = std::make_shared<bool>(false);
        std::shared_ptr<bool[]> is_done(new bool[n]{ 0 });
        std::shared_ptr<Variant[]> values(new Variant[n]());

        on_next_t _next = [=](int i) {
            has_value[i] = true;

            if (*has_value_all || all(has_value, n)) {
                observer->on_next(tuple(values, n));
            }

            // Would be way shorter with list arithmetic!
			// elif all([x for j, x in enumerate(is_done) if j != i]):
            else {
                bool completed = false;
                int j = 0;
                for (auto k = 0; k < n; k++) {
                    if (j == i) { j++; continue; }
                    if (!is_done[k]) { completed = false; break; }
                    j++;
                }
                if (completed) observer->on_completed();
            }

            *has_value_all = all(has_value, n);
        };

        auto done = [=](const Variant& i) {
            is_done[i] = true;
            if (all(is_done, n)) {
                observer->on_completed();
            }
        };

        auto subscriptions = std::make_shared<std::vector<std::shared_ptr<SingleAssignmentDisposable>>>(n);

        auto func = [=](int i) {
            (*subscriptions)[i] = std::make_shared<SingleAssignmentDisposable>();

            on_next_t on_next = [=](const Variant& x) {
                std::lock_guard guard(parent->lock);
                values[i] = x;
                _next(i);
            };

            on_completed_t on_completed = [=]() {
                std::lock_guard guard(parent->lock);
                done(i);
            };

            auto subscription = (*subscriptions)[i];
            assert(subscription);
            subscription->set_disposable(
                sources[i]->subscribe(
                    on_next, ONERROR_FWD(observer),
                    on_completed, scheduler_
                )
            );
        };

        for (int idx = 0; idx < n; idx++) {
            func(idx);
        }
        return std::make_shared<CompositeDisposable>(*subscriptions);
    };

    return Observable::get(subscribe);
}

rx_observable_t obs::combine_latest_(const rx_observable_t& sources...) {
    return combine_latest_({sources});
}

} // END namespace rx::observable