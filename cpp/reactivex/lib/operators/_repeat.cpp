#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::repeat_(int64_t repeat_count) {

    auto repeat = OP(source) {

        iterable_t gen;
        if (repeat_count < 0) {
            gen = std::make_shared<infinite_iterable>(source);
        }
        else {
            gen = std::make_shared<repeater>(repeat_count, source);
        }

        return defer(
            [gen](const scheduler_t&) { return concat_with_iterable(gen); }
        );

    };

    return repeat;
}

} // END namespace rx::observable::op
