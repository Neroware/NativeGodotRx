#ifndef RX_OBSERVABLE_OPERATORS_COMBINE_LATEST_H
#define RX_OBSERVABLE_OPERATORS_COMBINE_LATEST_H

#include "observable/observable.h"
#include "observable/combinelatest.h"

namespace rx::observable::op {

template <typename T>
static observable_op_t combine_latest_(const T& others_) {
    observable_vec_t others(others_.begin(), others_.end());

    observable_op_t combine_latest = OP(source) {
        return rx::observable::combine_latest_(insert_front(source, others));
    };

    return combine_latest;
}

template <typename... Args>
static observable_op_t combine_latest_(const Args&... others) {

    observable_op_t combine_latest = OP(source) {
        return rx::observable::combine_latest_(source, others...);
    };

    return combine_latest;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_COMBINE_LATEST_H
