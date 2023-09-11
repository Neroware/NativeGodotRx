#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::combine_latest_(const rx_observable_list_t& others) {

    observable_op_t combine_latest = OP(source) {
        observable_list_t sources(others.begin(), others.end());
        sources.push_front(source);
        return rx::observable::combine_latest(sources);
    };

    return combine_latest;
}

observable_op_t ops::combine_latest_(const rx_observable_t& others...) {

    observable_op_t combine_latest = OP(source) {
        return rx::observable::combine_latest(source, others);
    };

    return combine_latest;
}

} // END namespace rx::observable::op
