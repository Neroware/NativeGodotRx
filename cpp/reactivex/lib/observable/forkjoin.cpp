#include "observable/definitions.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::fork_join_(const rx_observable_list_t& sources_) {
    observable_vec_t sources(sources_.begin(), sources_.end());
    rx_observable_t parent = sources[0];

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        int n = sources.size();
        std::shared_ptr<Variant[]> values(new Variant[n]());
        std::shared_ptr<bool[]> is_done(new bool[n]{ 0 });
        std::shared_ptr<bool[]> has_value(new bool[n]{ 0 });

        auto done = [=](int i) {
            is_done[i] = true;

            if (!has_value[i]) {
                observer->on_completed();
                return;
            }

            if (all(is_done, n)) {
                if (all(has_value, n)) {
                    observer->on_next(tuple(values, n));
                    observer->on_completed();
                }
                else {
                    observer->on_completed();
                }
            }
        };

        auto subscriptions = std::make_shared<std::vector<std::shared_ptr<SingleAssignmentDisposable>>>(n);

        auto _subscribe = [=](int i) {
            (*subscriptions)[i] = std::make_shared<SingleAssignmentDisposable>();

            on_next_t on_next = [=](const Variant& value) {
                std::lock_guard guard(parent->lock);
                values[i] = value;
                has_value[i] = true;
            };

            on_completed_t on_completed = [=]() {
                std::lock_guard guard(parent->lock);
                done(i);
            };

            (*subscriptions)[i]->set_disposable(sources[i]->subscribe(
                on_next, ONERROR_FWD(observer),
                on_completed, scheduler_
            ));
        };

        for (int i = 0; i < n; i++) {
            _subscribe(i);
        }
        return std::make_shared<CompositeDisposable>(*subscriptions);

    };

    return Observable::get(subscribe);
}

rx_observable_t obs::fork_join_(const rx_observable_t& sources...) {
    return fork_join_(sources);
}

} // END namespace rx::observable