#include "observable/definitions.h"
#include "notification.h"
#include "wrapper/notificationwrapper.h"

namespace rx::observable::op {

observable_op_t ops::dematerialize_() {
    observable_op_t dematerialize = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            on_next_t on_next = [=](const notification_t& notification) {
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
