#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::zip_(const rx_observable_list_t& sources_) {

    observable_op_t zip = OP(source) {
        observable_list_t sources(sources_.begin(), sources_.end());
        sources.push_front(source);
        return rx::observable::zip(sources); 
    };

    return zip;
}

observable_op_t ops::zip_(const rx_observable_t& others...) {

    observable_op_t zip = OP(source) {
        return rx::observable::zip(source, others);
    };

    return zip;
}

observable_op_t ops::zip_with_iterable_(const iterable_t& seq) {
    observable_op_t zip_with_iterable = OP(source) {

        auto first = source;
        auto second = seq->iter();

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            on_next_t on_next = [=](const Variant& left) {
                try {
                    if (!(second->has_next())) {
                        observer->on_completed();
                    } else {
                        const Variant right = second->next();
                        Array result = Array::make(left, right);
                        observer->on_next(result);
                    }
                } catch (...) {
                    observer->on_error(std::current_exception());
                }
            };

            return first->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return zip_with_iterable;
}

} // END namespace rx::observable::op
