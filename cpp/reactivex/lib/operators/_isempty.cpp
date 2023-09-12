#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::is_empty_() {

    auto mapper = [](bool b) -> bool {
        return !b;
    };

    return pipe::compose(
        some(),
        map(mapper)
    );
}

} // END namespace rx::observable::op
