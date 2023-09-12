#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::reduce_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed) {

    if (IS_SET(seed)) {
        auto scanner = scan(accumulator, seed);
        return pipe::compose(
            scanner,
            last_or_default(seed)
        );
    }

    return pipe::compose(
        scan(accumulator),
        last()
    );
}

} // END namespace rx::observable::op
