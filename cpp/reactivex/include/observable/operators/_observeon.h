#ifndef RX_OBSERVABLE_OPERATORS_OBSERVEON_H
#define RX_OBSERVABLE_OPERATORS_OBSERVEON_H

#include "observable/observable.h"

#include "observer/observeonobserver.h"

using namespace rx::observer;

namespace rx::observable::op {

static observable_op_t observe_on_(const scheduler_t& scheduler) {
    observable_op_t observe_on = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, subscribe_scheduler = nullptr) {
            return source->subscribe(
                ObserveOnObserver::get(scheduler, observer), 
                subscribe_scheduler
            );
        };

        return Observable::get(subscribe);
    };
    
    return observe_on;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_OBSERVEON_H