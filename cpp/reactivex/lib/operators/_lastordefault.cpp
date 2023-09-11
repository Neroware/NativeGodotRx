#include "observable/definitions.h"

#include "exception/exception.h"

using namespace rx::exception;
using namespace rx::disposable;

namespace rx::observable::op {

rx_observable_t ops::last_or_default_async_(const rx_observable_t& source, bool has_default, const Variant& default_value) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        auto value = std::make_shared<Variant>(default_value);
        auto seen_value = std::make_shared<bool>(false);

        auto on_next = [=](const Variant& x) {
            *value = x;
            *seen_value = true;
        };

        auto on_completed = [=]() {
            if (!(*seen_value) && !has_default) {
                observer->on_error(std::make_exception_ptr(SequenceContainsNoElementsException()));
            } else {
                observer->on_next(*value);
                observer->on_completed();
            }
        };

        return source->subscribe(
            on_next, ONERROR_FWD(observer), on_completed, 
            scheduler);
    };

    return Observable::get(subscribe);
}


observable_op_t ops::last_or_default_(const Variant& default_value, const predicate_t<Variant>& predicate) {

    observable_op_t last_or_default = OP(source) {
        if (predicate) {
            return source->pipe(
                filter(predicate),
                last_or_default_(default_value)
            );
        }

        return last_or_default_async_(source, true, default_value);
    };

    return last_or_default;
}

} // END namespace rx::observable::op
