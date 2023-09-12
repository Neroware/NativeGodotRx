#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::find_value_(const predicate_t<Variant, uint64_t, rx_observable_t>& predicate, bool yield_index) {

    observable_op_t find_value = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto index = std::make_shared<uint64_t>(0) ;

            on_next_t on_next = [=](const Variant& x) {
                bool should_run = false;
                try {
                    should_run = predicate(x, *index, source);
                }
                catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                if (should_run) {
                    if (yield_index) {
                        observer->on_next(*index);
                    }
                    else {
                        observer->on_next(x);
                    }
                    observer->on_completed();
                }
                else {
                    ++(*index);
                }
            };

            on_completed_t on_completed = [=]() {
                if (yield_index) {
                    observer->on_next(-1);
                }
                else {
                    observer->on_next(VNULL);
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

    return find_value;
}

} // END namespace rx::observable::op
