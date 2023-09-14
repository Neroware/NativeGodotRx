#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::skip_(uint64_t count) {

    auto skip = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto remaining = std::make_shared<uint64_t>(count);
            
            on_next_t on_next = [=](const Variant& value) {
                if (*remaining <= 0) {
                    observer->on_next(value);
                } else {
                    (*remaining)--;
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return skip;
}

} // END namespace rx::observable::op
