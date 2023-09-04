#ifndef RX_OBSERVABLE_OPERATORS_MAXBY_H
#define RX_OBSERVABLE_OPERATORS_MAXBY_H

#include "observable/observable.h"

#include "observable/operators/_minby.h"

#include "internal/utils.h"
#include "typing.h"


namespace rx::observable::op {

static observable_op_t max_by_(
    const mapper_t<Variant, Variant>& key_mapper,
    const sub_comparer_t<Variant>& comparer = nullptr
) {

    sub_comparer_t<Variant> cmp = comparer ? comparer : basic::default_sub_comparer<int>;

    observable_op_t max_by = OP(source) {
        return extrema_by(source, key_mapper, cmp);
    };

    return max_by;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MAXBY_H
