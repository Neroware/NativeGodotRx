#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::max_by_(
    const mapper_t<Variant, Variant>& key_mapper,
    const sub_comparer_t<Variant>& comparer
) {

    sub_comparer_t<Variant> cmp = comparer ? comparer : basic::default_sub_comparer<int>;

    observable_op_t max_by = OP(source) {
        return extrema_by(source, key_mapper, cmp);
    };

    return max_by;
}

} // END namespace rx::observable::op

