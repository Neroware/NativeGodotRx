#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::retry_(int64_t retry_count) {
    auto retry = OP(source) {
        iterable_t gen;
        if (retry_count < 0) {
            gen = std::make_shared<infinite_iterable>(source);
        } else {
            gen = std::make_shared<repeater>(retry_count, source);
        }

        return catch_with_iterable(gen);
    };

    return retry;
}

} // END namespace rx::observable::op
