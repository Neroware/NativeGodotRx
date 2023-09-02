#ifndef RX_OBSERVABLE_OPERATORS_FIRST_H
#define RX_OBSERVABLE_OPERATORS_FIRST_H

#include "observable/observable.h"
#include "observable/operators/_firstordefault.h"
#include "observable/operators/_filter.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t first_(const predicate_t<Variant>& predicate = nullptr) {

    if (predicate) {
        return pipe::compose(filter_(predicate), first_());
    }
    return first_or_default_async_(false);
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_FIRST_H
