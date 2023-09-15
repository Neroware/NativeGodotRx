#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::do_while_(const predicate_t<rx_observable_t>& condition) {

    auto do_while = OP(source) {
        return source->pipe(
            concat(
                source->pipe(
                    while_do(condition)
                )
            )
        );
    };

    return do_while;

}

}