#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::take_while_(const predicate_t<Variant>& predicate, bool inclusive) {

    observable_op_t take_while = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto running = std::make_shared<bool>(true);

            auto on_next = [=](const Variant& value) {
                {
                    std::lock_guard<RLock> guard(source->lock);

                    if (!*running) {
                        return;
                    }

                    try {
                        *running = predicate(value);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                }

                if (*running) {
                    observer->on_next(value);
                } 
                else {
                    if (inclusive) {
                        observer->on_next(value);
                    }
                    observer->on_completed();
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return take_while;
}

observable_op_t ops::take_while_indexed_(const predicate_indexed_t<Variant>& predicate, bool inclusive) {

    observable_op_t take_while_indexed = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto running = std::make_shared<bool>(true);
            auto i = std::make_shared<size_t>(0);

            auto on_next = [=](const Variant& value) {
                if (!*running) {
                    return;
                }

                try {
                    *running = predicate(value, *i);
                    ++(*i);
                } catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                if (*running) {
                    observer->on_next(value);
                } else {
                    if (inclusive) {
                        observer->on_next(value);
                    }
                    observer->on_completed();
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return take_while_indexed;
}

} // END namespace rx::observable::op
