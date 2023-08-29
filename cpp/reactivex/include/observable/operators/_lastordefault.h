#ifndef RX_OBSERVABLE_OPERATORS_LASTORDEFAULT_H
#define RX_OBSERVABLE_OPERATORS_LASTORDEFAULT_H

#include "observable/observable.h"
#include "internal/utils.h"
#include "typing.h"

#include "exception/exception.h"

#include "observable/operators/_filter.h"

using namespace rx::exception;
using namespace rx::disposable;

namespace rx::observable {

std::shared_ptr<Observable> last_or_default_async_(
    const std::shared_ptr<Observable>& source,
    bool has_default = false,
    const Variant& default_value = VNULL) {

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


observable_op_t last_or_default_(
    const Variant& default_value = VNULL,
    const predicate_t<Variant>& predicate = nullptr) {

    observable_op_t last_or_default = OP(source) {
        if (predicate) {
            return source->pipe(
                filter_(predicate),
                last_or_default_(default_value)
            );
        }

        return last_or_default_async_(source, true, default_value);
    };

    return last_or_default;
}


} // END namespace rx::observable



#endif // RX_OBSERVABLE_OPERATORS_LASTORDEFAULT_H
