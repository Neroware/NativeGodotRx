#include "observable/definitions.h"

#include "exception/exception.h"

using namespace rx::exception;
using namespace rx::disposable;

namespace rx::observable::op {

observable_op_t ops::last_(const predicate_t<Variant>& predicate) {
    observable_op_t last = OP(source) {
        if (predicate) {
            return source->pipe(
                filter(predicate),
                last_()
            );
        }

        return last_or_default_async(source, false);
    };

    return last;
}

} // END namespace rx::observable::op
