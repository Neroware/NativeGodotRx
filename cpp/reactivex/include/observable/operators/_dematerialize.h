#ifndef RX_OBSERVABLE_OPERATORS_DEMATERIALIZIZE_H
#define RX_OBSERVABLE_OPERATORS_DEMATERIALIZIZE_H

#include "observable/observable.h"
#include "notification.h"
#include "wrapper/notificationwrapper.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t dematerialize_() {
    observable_op_t dematerialize = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto on_next = [=](Ref<RxNotification> value) {
                auto notification = RxNotification::unwrap(value);
                notification->accept(observer);
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return dematerialize;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_DEMATERIALIZIZE_H
