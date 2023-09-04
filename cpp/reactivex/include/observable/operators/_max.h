#ifndef RX_OBSERVABLE_OPERATORS_MAX_H
#define RX_OBSERVABLE_OPERATORS_MAX_H

#include "observable/observable.h"
#include "observable/operators/_maxby.h"
#include "observable/operators/_min.h"
#include "observable/operators/_map.h"
#include "internal/utils.h"

namespace rx::observable::op {

static observable_op_t max_(
    const comparer_t<Variant>& comparer = nullptr
) {

    return pipe::compose(
        max_by_(basic::identity<const Variant&>, comparer),
        map_(first_only)
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MAX_H