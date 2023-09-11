#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::filter_(const predicate_t<Variant>& predicate) {

    observable_op_t filter = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler) {

            on_next_t on_next = [=](const Variant& value) {
                bool should_run;
                try {
                    should_run = predicate(value);
                }
                catch(...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                if (should_run) {
                    observer->on_next(value);
                }
            };
            
            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return filter;
}

observable_op_t ops::filter_indexed_(const predicate_indexed_t<Variant>& predicate) {

    observable_op_t filter = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler) {

            auto count = std::make_shared<uint64_t>(0);

            on_next_t on_next = [=](const Variant& value) {
                bool should_run = true;
                if (predicate) {
                    try {
                        should_run = predicate(value, *count);
                    }
                    catch(...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                    (*count)++;
                }

                if (should_run) {
                    observer->on_next(value);
                }
            };
            
            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return filter;
}

} // END namespace rx::observable::op