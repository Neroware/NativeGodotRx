#include "observable/definitions.h"
#include "pipe.h"

namespace rx::observable::op {

observable_op_t ops::all_(const predicate_t<Variant>& predicate) {

    auto _filter = [=](const Variant& v) {
        return !predicate(v);
    };

    auto _mapping = [](bool b) -> bool {
        return !b;
    };

    return pipe::compose(
        filter(_filter),
        some(),
        map(_mapping)
    );
    
}

} // END namespace rx::observable::op
