#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::first_(const predicate_t<Variant>& predicate) {

    if (predicate) {
        return pipe::compose(filter(predicate), first_());
    }
    return first_or_default_async(false);
}

} // END namespace rx::observable::op
