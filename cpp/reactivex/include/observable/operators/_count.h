#ifndef RX_OBSERVABLE_OPERATORS_COUNT_H
#define RX_OBSERVABLE_OPERATORS_COUNT_H

#include "observable/observable.h"
#include "observable/operators/_filter.h"
#include "observable/operators/_count.h"
#include "observable/operators/_reduce.h"
#include "pipe.h"

#include "typing.h"

namespace rx::observable::op {

observable_op_t count_(const predicate_t<Variant>& predicate = nullptr) {

    if (predicate) {
        return pipe::compose(
            filter_(predicate),
            count_()
        );
    }

    auto reducer = [](int n, const Variant&) -> int {
        return n + 1;
    };

    auto counter = reduce_(reducer, 0);
    return counter;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_COUNT_H
