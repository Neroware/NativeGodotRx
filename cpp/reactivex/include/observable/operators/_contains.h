#ifndef RX_OBSERVABLE_OPERATORS_CONTAINS_H
#define RX_OBSERVABLE_OPERATORS_CONTAINS_H

#include "observable/observable.h"
#include "observable/operators/_filter.h"
#include "observable/operators/_some.h"

#include "basic.h"
#include "pipe.h"
#include "typing.h"

namespace rx::observable::op {

observable_op_t contains_(const Variant& value, const comparer_t<Variant>& comparer = nullptr) {

    auto comparer_ = comparer ? comparer : basic::default_comparer<const Variant&>;

    auto predicate = [=](const Variant& v) -> bool {
        return comparer_(v, value);
    };

    return pipe::compose(
        filter_(predicate),
        some_()
    );
    
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_CONTAINS_H
