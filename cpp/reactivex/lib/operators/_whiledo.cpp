#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::while_do_(const predicate_t<rx_observable_t>& condition) {

    observable_op_t while_do = OP(source) {
        auto obs = source;

        auto it = std::make_shared<while_iterable>(condition, std::make_shared<infinite_iterable>(obs));

        return concat_with_iterable(it);
    };

    return while_do;
}

} // END namespace rx::observable::op
