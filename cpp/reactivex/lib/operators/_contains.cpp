#include "observable/definitions.h"

namespace rx::observable::op {

static observable_op_t contains_(const Variant& value, const comparer_t<Variant>& comparer = nullptr) {

    auto comparer_ = comparer ? comparer : basic::default_comparer<const Variant&>;

    auto predicate = [=](const Variant& v) -> bool {
        return comparer_(v, value);
    };

    return pipe::compose(
        filter(predicate),
        some()
    );
    
}

} // END namespace rx::observable::op

