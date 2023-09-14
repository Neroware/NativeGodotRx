#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::single_(const predicate_t<Variant>& predicate) {
    
    if (predicate) {
        return pipe::compose(filter(predicate), single_());
    }
    return single_or_default_async(false);
    
}

} // END namespace rx::observable::op
