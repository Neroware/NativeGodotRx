#include "observable/definitions.h"

namespace rx::observable::op {

static Variant first_only(const Array& xs) {
    if (xs.is_empty()) {
        throw SequenceContainsNoElementsException();
    }
    return xs[0]; 
}

observable_op_t ops::max_(
    const comparer_t<Variant>& comparer
) {

    return pipe::compose(
        max_by(basic::identity<const Variant&>, comparer),
        map(first_only)
    );
}

} // END namespace rx::observable::op
