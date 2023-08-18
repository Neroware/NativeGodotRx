#ifndef RX_OBSERVABLE_GENERATE_H
#define RX_OBSERVABLE_GENERATE_H

#include "observable/observable.h"

#include "typing.h"
#include "disposable/multiassignmentdisposable.h"

using namespace rx::disposable;

namespace rx::observable {

static std::shared_ptr<Observable> generate_(
    const Variant& initial_state, 
    const predicate_t<const Variant&>& condition,
    const mapper_t<Variant, const Variant&>& iterate
) {

    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();
        auto first = std::make_shared<bool>(true);
        auto state0 = std::make_shared<Variant>(initial_state);
        auto mad = std::make_shared<MultiAssignmentDisposable>();

        auto action = RECURSIVE_ACTION {
            bool has_result = false;
            Variant result;

            try {
                if (*first) {
                    *first = false;
                }
                else {
                    *state0 = iterate(*state0);
                }

                has_result = condition(*state0);
                if (has_result) {
                    result = *state0;
                }
            }
            catch(...) {
                observer->on_error(std::current_exception());
                return nullptr;
            }

            if (has_result) {
                observer->on_next(result);
                mad->set_disposable(scheduler->schedule(ACTION { return _action(scheduler__, state, _action); }));
            }
            else {
                observer->on_completed();
            }

            return nullptr;
        };

        mad->set_disposable(scheduler->schedule(ACTION { return action(scheduler__, state, action); }));
        return mad;
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_GENERATE_H