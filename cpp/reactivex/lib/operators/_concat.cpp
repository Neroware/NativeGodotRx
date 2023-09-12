#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::concat_(const rx_observable_list_t& sources) {

    observable_op_t concat = OP(source) {
        observable_list_t _sources(sources.begin(), sources.end());
        _sources.push_front(source);
        return rx::observable::concat(_sources);
    };

    return concat;
}

observable_op_t ops::concat_(const rx_observable_t& others...) {

    observable_op_t concat = OP(source) {
        return rx::observable::concat(source, others);
    };

    return concat;
}

} // END namespace rx::observable::op
