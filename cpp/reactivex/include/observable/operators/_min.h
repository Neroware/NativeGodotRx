#ifndef RX_OBSERVABLE_OPERATORS_MIN_H
#define RX_OBSERVABLE_OPERATORS_MIN_H

#include "observable/observable.h"
#include "observable/operators/_minby.h"
#include "observable/operators/_map.h"
#include "internal/utils.h"

namespace rx::observable::op {

static Variant first_only(const Array& xs) {
    if (xs.is_empty()) {
        throw SequenceContainsNoElementsException();
    }
    return xs[0]; 
}

static observable_op_t min_(
    const comparer_t<Variant>& comparer = nullptr
) {

    return pipe::compose(
        min_by_(basic::identity<const Variant&>, comparer),
        map_(first_only)
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MIN_H
