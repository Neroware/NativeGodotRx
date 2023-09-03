#ifndef RX_OBSERVABLE_OPERATORS_ISEMPTY_H
#define RX_OBSERVABLE_OPERATORS_ISEMPTY_H

#include "observable/observable.h"
#include "observable/operators/_some.h"
#include "observable/operators/_map.h"

namespace rx::observable::op {

static observable_op_t is_empty_() {

    auto mapper = [](bool b) -> bool {
        return !b;
    };

    return pipe::compose(
        some_(),
        map_(mapper)
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ISEMPTY_H
