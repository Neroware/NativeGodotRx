#ifndef RX_OBSERVABLE_OPERATORS_DISTINCTUNTILCHANGED_H
#define RX_OBSERVABLE_OPERATORS_DISTINCTUNTILCHANGED_H

#include "observable/observable.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t distinct_until_changed_(const mapper_t<Variant, Variant>& key_mapper = nullptr, const comparer_t<Variant>& comparer = nullptr) {

    mapper_t<Variant, Variant> key_mapper_ = key_mapper ? key_mapper : basic::identity<const Variant&>;
    comparer_t<Variant> comparer_ = comparer ? comparer : basic::default_comparer<const Variant&>;

    observable_op_t distinct_until_changed = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto has_current_key = std::make_shared<bool>(false);
            auto current_key = std::shared_ptr<Variant>();

            on_next_t on_next = [=](const Variant& value) {
                bool comparer_equals = false;
                Variant key;

                try {
                    key = key_mapper_(value);
                }
                catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                if (*has_current_key) {
                    try {
                        comparer_equals = comparer_(*current_key, key);
                    }
                    catch (...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                }

                if (!*has_current_key || !comparer_equals) {
                    *has_current_key = true;
                    *current_key = key;
                    observer->on_next(value);
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler_
            );
        };

        return Observable::get(subscribe);
    };

    return distinct_until_changed;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_DISTINCTUNTILCHANGED_H
