#include "observable/definitions.h"

#include "typing.h"

using namespace rx::disposable;

namespace rx::observable {

rx_observable_t obs::if_then_(
    const predicate_t<>& condition,
    const rx_observable_t& then_source,
    const rx_observable_t& else_source
) {

    auto else_source_ = else_source ? else_source : empty();

    observable_factory_t factory = [=](const scheduler_t & scheduler) -> rx_observable_t {
        return condition() ? then_source : else_source_;
    };

    return defer_(factory);
}

}