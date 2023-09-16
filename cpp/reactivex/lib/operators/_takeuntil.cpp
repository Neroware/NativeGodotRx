#include "observable/definitions.h"

#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::take_until_(const rx_observable_t& other) {

    observable_op_t take_until = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

            auto on_completed = [=](const Variant& _ = VNULL) {
                observer->on_completed();
                return nullptr;
            };

            return std::make_shared<CompositeDisposable>(
                source->subscribe(observer, scheduler),
                other->subscribe(
                    on_completed, ONERROR_FWD(observer), 
                    basic::noop<>, scheduler
                )
            );
        };

        return Observable::get(subscribe);
    };

    return take_until;
}

} // END namespace rx::observable::op
