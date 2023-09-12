#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::with_latest_from_(const rx_observable_list_t& sources_) {

    observable_vec_t sources(sources_.begin(), sources_.end());

    observable_op_t with_latest_from = OP(source) {
        return rx::observable::with_latest_from(source, sources);
    };

    return with_latest_from;
}

observable_op_t ops::with_latest_from_(const rx_observable_t& others...) {

    observable_op_t zip = OP(source) {
        return rx::observable::with_latest_from(source, others);
    };

    return zip;
}

} // END namespace rx::observable::op
