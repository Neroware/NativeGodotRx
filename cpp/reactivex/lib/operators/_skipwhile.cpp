#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::skip_while_(const predicate_t<Variant>& predicate) {

    observable_op_t skip_while = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto running = std::make_shared<bool>(false);

            auto on_next = [=](const Variant& value) {
                if (!*running) {
                    try {
                        *running = !predicate(value);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                }

                if (*running) {
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

    return skip_while;
}

observable_op_t ops::skip_while_indexed_(const predicate_indexed_t<Variant>& predicate) {
    auto indexer = [](const Variant& x, int64_t i) -> Array {
        return Array::make(x, i);
    };
    auto skipper = [predicate](const Array& x) -> bool {
        return predicate(x[0], x[1]);
    };
    auto mapper = [](const Array& x) -> Variant {
        return x[0];
    };

    return pipe::compose(
        map_indexed(indexer),
        skip_while_(skipper),
        map(mapper)
    );
}

} // END namespace rx::observable::op