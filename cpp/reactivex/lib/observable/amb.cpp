#include "observable/definitions.h"

namespace rx::observable {

rx_observable_t obs::amb_(const rx_observable_list_t& sources) {

    auto acc = never();

    auto func = [](const rx_observable_t& previous, const rx_observable_t& current) -> rx_observable_t {
        return op::amb(previous)(current);
    };

    for (const auto& source : sources) {
        acc = func(acc, source);
    }

    return acc;

}

} // END namespace rx::observable