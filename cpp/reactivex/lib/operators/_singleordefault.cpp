#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::single_or_default_async_(bool has_default, const Variant& default_value) {

    auto single_or_default_async = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

            auto seen_value = std::make_shared<bool>(false);
            auto value = std::make_shared<Variant>(default_value);

            on_next_t on_next = [=](const Variant& x) {
                if (*seen_value) {
                    observer->on_error(std::make_exception_ptr(SequenceContainsNoElementsException()));
                } else {
                    *value = x;
                    *seen_value = true;
                }
            };

            on_completed_t on_completed = [=]() {
                if (!*seen_value && !has_default) {
                    observer->on_error(std::make_exception_ptr(SequenceContainsNoElementsException()));
                } else {
                    observer->on_next(*value);
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
    return single_or_default_async;
}

observable_op_t ops::single_or_default_(const predicate_t<Variant>& predicate, const Variant& default_value) {
    if (predicate) {
        return pipe::compose(
            filter(predicate), 
            ops::single_or_default_(nullptr, default_value)
        );
    }
    return single_or_default_async_(true, default_value);
}

} // END namespace rx::observable::op
