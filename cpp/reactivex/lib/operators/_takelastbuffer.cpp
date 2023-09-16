#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::take_last_buffer_(uint64_t count) {

    observable_op_t take_last_buffer = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto q = std::make_shared<Array>();

            auto on_next = [=](const Variant& x) {
                {
                    std::lock_guard<RLock> guard(source->lock);
                    q->push_back(x);
                    if (q->size() > count) {
                        q->pop_front();
                    }
                }
            };

            auto on_completed = [=]() {
                observer->on_next(*q);
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return take_last_buffer;
}

} // END namespace rx::observable::op
