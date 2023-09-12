#include "observable/definitions.h"

using namespace rx::exception;

namespace rx::observable::op {

observable_op_t ops::first_or_default_async_(bool has_default, const Variant& default_value) {

    observable_op_t first_or_default_async = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            on_next_t on_next = [=](const Variant& x) {
                observer->on_next(x);
                observer->on_completed();
            };

            on_completed_t on_completed = [=]() {
                if (!has_default) {
                    observer->on_error(std::make_exception_ptr(SequenceContainsNoElementsException()));
                }
                else {
                    observer->on_next(default_value);
                    observer->on_completed();
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return first_or_default_async;
}

observable_op_t ops::first_or_default_(const predicate_t<Variant>& predicate, const Variant& default_value) {

    if (predicate) {
        return pipe::compose(filter(predicate), first_or_default_async_(true, default_value));
    }
    return first_or_default_async_(true, default_value);
}

} // END namespace rx::observable::op
