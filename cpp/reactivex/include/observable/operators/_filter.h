#ifndef RX_OBSERVABLE_OPERATORS_FILTER_H
#define RX_OBSERVABLE_OPERATORS_FILTER_H

#include "observable/observable.h"

#include "typing.h"

namespace rx::observable {

static observable_op_t filter_(const predicate_t<Variant>& predicate) {

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

static observable_op_t filter_indexed_(const predicate_indexed_t<Variant>& predicate = nullptr) {

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

} // END namespace rx::observable

#endif // RX_OBSERVABLE_OPERATORS_FILTER_H