#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::to_list_() {

    observable_op_t to_list = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

            auto queue = std::make_shared<Array>();

            auto on_next = [=](const Variant& x) {
                queue->push_back(x);
            };

            auto on_completed = [=]() {
                observer->on_next(*queue);
                *queue = Array();
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return to_list;
}

observable_op_t ops::to_iterable_() {
    auto mapper = [](const Array& value) -> Variant {
        return iterator::to_iterable(value);
    };

    return pipe::compose(
        to_list_(),
        map(mapper)
    );
}

} // END namespace rx::observable::op
