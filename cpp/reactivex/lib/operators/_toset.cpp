#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::to_set_(const comparer_t<Variant>& comparer) {
    return pipe::compose(
        distinct(nullptr, comparer),
        to_list()
    );
}

}