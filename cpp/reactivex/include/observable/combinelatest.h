#ifndef RX_OBSERVABLE_COMBINELATEST_H
#define RX_OBSERVABLE_COMBINELATEST_H

#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

#include "internal/utils.h"

#include <cassert>

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

template<typename IterableT>
static std::shared_ptr<Observable> combine_latest_(const IterableT& sources_) {
    observable_vec_t sources(sources_.begin(), sources_.end());

    std::shared_ptr<Observable> parent = sources[0];

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
                    on_next, 
                    [observer] (const std::exception_ptr& e) { observer->on_error(e); },
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

template<typename... Args>
static std::shared_ptr<Observable> combine_latest_(const Args&... sources) {
    std::vector<std::shared_ptr<Observable>> args = {sources...};
    return combine_latest_(args);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_COMBINELATEST_H