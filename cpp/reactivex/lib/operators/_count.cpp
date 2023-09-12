#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::count_(const predicate_t<Variant>& predicate) {

    if (predicate) {
        return pipe::compose(
            filter(predicate),
            count_()
        );
    }

    auto reducer = [](int n, const Variant&) -> int {
        return n + 1;
    };

    auto counter = reduce(reducer, 0);
    return counter;
}

} // END namespace rx::observable::op
