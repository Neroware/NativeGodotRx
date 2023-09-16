#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::take_last_(uint64_t count) {

    observable_op_t take_last = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto q = std::make_shared<std::list<Variant>>();

            auto on_next = [=](const Variant& x) {
                q->push_back(x);
                if (q->size() > count) {
                    q->pop_front();
                }
            };

            auto on_completed = [=]() {
                while (!q->empty()) {
                    observer->on_next(q->front());
                    q->pop_front();
                }
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return take_last;
}

} // END namespace rx::observable::op
