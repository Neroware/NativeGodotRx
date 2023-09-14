#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::delay_subscription_(const time_delta_t& duetime, const scheduler_t& scheduler) {
    observable_op_t delay_subscription = OP(source) {

        auto mapper = [](const Variant&) -> rx_observable_t {
            return empty();
        };

        return source->pipe(
            delay_with_mapper(timer(duetime, scheduler), mapper)
        );
    };

    return delay_subscription;
}

} // END namespace rx::observable::op
