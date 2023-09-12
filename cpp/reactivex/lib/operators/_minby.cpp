#include "observable/definitions.h"

namespace rx::observable::op {

rx_observable_t ops::extrema_by(
    const rx_observable_t& source,
    const mapper_t<Variant, Variant>& key_mapper,
    const sub_comparer_t<Variant>& comparer
) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        auto has_value = std::make_shared<bool>(false);
        auto last_key = std::make_shared<Variant>();
        auto items = std::make_shared<Array>();

        on_next_t on_next = [=](const Variant& x) {
            Variant key;
            try {
                key = key_mapper(x);
            }
            catch (...) {
                observer->on_error(std::current_exception());
                return;
            }

            int comparison = 0;

            if (!*has_value) {
                *has_value = true;
                *last_key = key;
            } else {
                try {
                    comparison = comparer(key, *last_key);
                }
                catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }
            }

            if (comparison > 0) {
                *last_key = key;
                items->clear();
            }

            if (comparison >= 0) {
                items->push_back(x);
            }
        };

        on_completed_t on_completed = [=]() {
            observer->on_next(*items);
            observer->on_completed();
        };

        return source->subscribe(
            on_next, ONERROR_FWD(observer),
            on_completed, scheduler
        );
    };

    return Observable::get(subscribe);;
}

observable_op_t ops::min_by_(
    const mapper_t<Variant, Variant>& key_mapper,
    const sub_comparer_t<Variant>& comparer
) {

    sub_comparer_t<Variant> cmp = comparer ? comparer : basic::default_sub_comparer<int>;

    observable_op_t min_by = OP(source) {
        return extrema_by(
            source, 
            key_mapper, 
            [cmp](const Variant& x, const Variant& y) -> int { return -cmp(x, y); }
        );
    };

    return min_by;
}

} // END namespace rx::observable::op
