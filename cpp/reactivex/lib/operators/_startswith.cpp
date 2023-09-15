#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::start_with_(const RxList<Variant>& args) {

    observable_op_t start_with = OP(source) {
        auto start = from_iterable(args.iter());
        return rx::observable::concat(start, source);
    };

    return start_with;

}

observable_op_t ops::start_with_(const Variant& args...) {
    return ops::start_with_({args});
}

}