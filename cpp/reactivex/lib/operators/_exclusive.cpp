#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::exclusive_() {
    observable_op_t exclusive = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto has_current = std::make_shared<bool>(false);
            auto is_stopped = std::make_shared<bool>(false);
            auto m = std::make_shared<SingleAssignmentDisposable>();
            auto g = std::make_shared<CompositeDisposable>();
            
            g->add(m);

            on_next_t on_next = [=](const rx_observable_t& inner_source) {
                if (!*has_current) {
                    *has_current = true;

                    auto inner_subscription = std::make_shared<SingleAssignmentDisposable>();
                    g->add(inner_subscription);

                    on_completed_t on_completed_inner = [=]() {
                        g->remove(inner_subscription);
                        *has_current = false;
                        if (*is_stopped && g->size() == 1) {
                            observer->on_completed();
                        }
                    };

                    inner_subscription->set_disposable(
                        inner_source->subscribe(
                            ONNEXT_FWD(observer), ONERROR_FWD(observer),
                            on_completed_inner,
                            scheduler
                        )
                    );
                };
            };

            on_completed_t on_completed = [=]() {
                *is_stopped = true;
                if (!*has_current && g->size() == 1) {
                    observer->on_completed();
                }
            };

            m->set_disposable(source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            ));
            return g;
        };

        return Observable::get(subscribe);
    };

    return exclusive;
}

} // END namespace rx::observable::op
