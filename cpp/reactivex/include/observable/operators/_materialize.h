#ifndef RX_OBSERVABLE_OPERATORS_MATERIALIZE_H
#define RX_OBSERVABLE_OPERATORS_MATERIALIZE_H

#include "observable/observable.h"
#include "observer/observer.h"
#include "notification.h"

#include "wrapper/notificationwrapper.h"

namespace rx::observable::op {

static observable_op_t materialize_() {

    observable_op_t materialize = OP(source) {
        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

            on_next_t on_next = [=](const Variant& value) {
                observer->on_next(NOTIFICATION_NEXT(value));
            };

            on_error_t on_error = [=](const std::exception_ptr& exn) {
                observer->on_next(NOTIFICATION_ERROR(exn));
                observer->on_completed();
            };

            on_completed_t on_completed = [=]() {
                observer->on_next(NOTIFICATION_COMPLETED());
                observer->on_completed();
            };

            return source->subscribe(
                on_next, on_error, on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return materialize;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MATERIALIZE_H
