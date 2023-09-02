#ifndef RX_OBSERVABLE_OPERATORS_FINALLYACTION_H
#define RX_OBSERVABLE_OPERATORS_FINALLYACTION_H

#include "observable/observable.h"
#include "disposable/disposable.h"
#include "scheduler/immediatescheduler.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t finally_action_(const std::function<void()>& action) {
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

#endif // RX_OBSERVABLE_OPERATORS_FINALLY_ACTION_H
