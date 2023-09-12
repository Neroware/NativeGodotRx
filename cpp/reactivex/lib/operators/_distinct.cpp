#include "observable/definitions.h"

namespace rx::observable::op {

static int64_t array_index_of_comparer(const Array& array, const Variant& item, const comparer_t<Variant>& comparer) {
    for (int64_t i = 0; i < array.size(); i++) {
        if (comparer(array[i], item)) {
            return i;
        }
    }
    return -1;
}

struct rx_hash_set {
    mutable Array set;
    comparer_t<Variant> comparer;

    rx_hash_set(const comparer_t<Variant>& comparer_)
        : comparer(comparer_) {}

    bool push(const Variant& value) const {
        bool ret_value = array_index_of_comparer(this->set, value, this->comparer) == -1;
        if (ret_value) {
            this->set.append(value);
        }
        return ret_value;
    }
};

observable_op_t ops::distinct_(const mapper_t<Variant, Variant>& key_mapper, const comparer_t<Variant>& comparer) {
    comparer_t<Variant> comparer_ = comparer ? comparer : basic::default_comparer<const Variant&>;

    observable_op_t distinct = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            rx_hash_set hashset(comparer_);

            on_next_t on_next = [=](const Variant& x) {
                Variant key = x;

                if (key_mapper) {
                    try {
                        key = key_mapper(x);
                    }
                    catch (...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                }

                if (hashset.push(key)) {
                    observer->on_next(x);
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler_
            );
        };

        return Observable::get(subscribe);
    };

    return distinct;
}

} // END namespace rx::observable::op
