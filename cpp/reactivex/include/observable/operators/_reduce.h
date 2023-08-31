#ifndef RX_OBSERVABLE_OPERATORS_REDUCE_H
#define RX_OBSERVABLE_OPERATORS_REDUCE_H

#include "observable/observable.h"
#include "observable/operators/_scan.h"
#include "observable/operators/_lastordefault.h"
#include "observable/operators/_last.h"
#include "internal/utils.h"

#include "typing.h"
#include "pipe.h"

namespace rx::observable::op {

observable_op_t reduce_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) {

    if (IS_SET(seed)) {
        auto scanner = scan_(accumulator, seed);
        return pipe::compose(
            scanner,
            last_or_default_(seed)
        );
    }

    return pipe::compose(
        scan_(accumulator),
        last_()
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_REDUCE_H
