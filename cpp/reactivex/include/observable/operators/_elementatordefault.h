#ifndef RX_OBSERVABLE_OPERATORS_ELEMENTATORDEFAULT_H
#define RX_OBSERVABLE_OPERATORS_ELEMENTATORDEFAULT_H

#include "observable/observable.h"
#include "internal/utils.h"
#include "exception/exception.h"

using namespace rx::exception;

#include "typing.h"

namespace rx::observable::op {

static observable_op_t element_at_or_default_(uint64_t index, bool has_default = false, const Variant& default_value = VNULL) {
    if (index < 0) {
        throw ArgumentOutOfRangeException();
    }

    observable_op_t element_at_or_default = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto index_ = std::make_shared<uint64_t>(index);

            on_next_t on_next = [=](const Variant& x) {
                bool found = false;
                {
                    std::lock_guard<RLock> lock(source->lock);
                    if (*index_ == 0) {
                        found = true;
                    } else {
                        (*index_)--;
                    }
                }

                if (found) {
                    observer->on_next(x);
                    observer->on_completed();
                }
            };

            on_completed_t on_completed = [=]() {
                if (!has_default) {
                    observer->on_error(std::make_exception_ptr(ArgumentOutOfRangeException()));
                } else {
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

    return element_at_or_default;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ELEMENTATORDEFAULT_H
