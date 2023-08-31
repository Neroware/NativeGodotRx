#ifndef RX_OBSERVABLE_OPERATORS_CONCAT_H
#define RX_OBSERVABLE_OPERATORS_CONCAT_H

#include "observable/observable.h"
#include "observable/concat.h"

#include "typing.h"

namespace rx::observable::op {

template<typename T>
static observable_op_t concat_(const T& sources_) {
    observable_vec_t sources(sources_.begin(), sources_.end());

    observable_op_t concat = OP(source) {
        return concat_with_iterable_(insert_front(source, sources));
    };

    return concat;
}

template<typename... Args>
static observable_op_t concat_(const Args&... others) {

    observable_op_t concat = OP(source) {
        return concat_with_variadic_(source, others...);
    };

    return concat;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_CONCAT_H
