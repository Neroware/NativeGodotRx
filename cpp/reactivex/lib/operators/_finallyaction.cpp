#include "observable/definitions.h"

#include "disposable/disposable.h"
#include "scheduler/immediatescheduler.h"

namespace rx::observable::op {

observable_op_t ops::finally_action_(const action_t& action) {
    observable_op_t finally_action = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            disposable_t subscription;
            try {
                subscription = source->subscribe(observer, scheduler);
            } catch (...) {
                action();
                throw;
            }

            dispose_t dispose = [subscription, action]() {
                try {
                    subscription->dispose();
                } catch (...) {
                    // Ignore any exceptions during disposal.
                }
                action();
            };
            return std::make_shared<Disposable>(dispose);
        };

        return Observable::get(subscribe);
    };

    return finally_action;
}

} // END namespace rx::observable::op
