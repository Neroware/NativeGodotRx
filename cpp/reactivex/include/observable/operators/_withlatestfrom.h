#ifndef RX_OBSERVABLE_OPERATORS_WITHLATESTFROM_H
#define RX_OBSERVABLE_OPERATORS_WITHLATESTFROM_H

#include "observable/observable.h"
#include "observable/withlatestfrom.h"

namespace rx::observable::op {

template<typename IterableT>
static observable_op_t with_latest_from_(const IterableT& sources_) {

    observable_vec_t sources(sources_.begin(), sources_.end());

    observable_op_t with_latest_from = OP(source) {
        return rx::observable::with_latest_from_(source, sources);
    };

    return with_latest_from;
}
template <typename... Args>
static observable_op_t with_latest_from_(const Args&... others) {

    observable_op_t zip = OP(source) {
        return rx::observable::with_latest_from_(source, others...);
    };

    return zip;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_WITHLATESTFROM_H
