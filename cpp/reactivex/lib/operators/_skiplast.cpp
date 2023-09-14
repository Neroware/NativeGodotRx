#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::skip_last_(uint64_t count) {
    auto skip_last = OP(source) {
        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto q = std::make_shared<std::list<Variant>>();

            on_next_t on_next = [=](const Variant& value) {
                Variant front = UNSET;
                {
                    std::lock_guard<RLock> guard(source->lock);
                    q->push_back(value);
                    if (q->size() > count) {
                        front = q->front();
                        q->pop_front();
                    }
                }
                
                if (IS_SET(front)) {
                    observer->on_next(front);
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return skip_last;
}

} // END namespace rx::observable::op
