#ifndef RX_OBSERVABLE_OPERATORS_LAST_H
#define RX_OBSERVABLE_OPERATORS_LAST_H

#include "observable/observable.h"
#include "internal/utils.h"
#include "typing.h"

#include "exception/exception.h"

#include "observable/operators/_filter.h"
#include "observable/operators/_lastordefault.h"

using namespace rx::exception;
using namespace rx::disposable;

namespace rx::observable::op {

static observable_op_t last_(const predicate_t<Variant>& predicate = nullptr) {
    observable_op_t last = OP(source) {
        if (predicate) {
            return source->pipe(
                filter_(predicate),
                last_()
            );
        }

        return last_or_default_async_(source, false);
    };

    return last;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_LAST_H
