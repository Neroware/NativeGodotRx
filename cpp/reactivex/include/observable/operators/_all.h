#ifndef RX_OBSERVABLE_OPERATORS_ALL_H
#define RX_OBSERVABLE_OPERATORS_ALL_H

#include "observable/observable.h"
#include "observable/operators/_filter.h"
#include "observable/operators/_some.h"
#include "observable/operators/_map.h"
#include "pipe.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t all_(const predicate_t<Variant>& predicate) {

    auto filter = [=](const Variant& v) {
        return !predicate(v);
    };

    auto mapping = [](bool b) -> bool {
        return !b;
    };

    return pipe::compose(
        filter_(filter),
        some_(),
        map_(mapping)
    );
    
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ALL_H
