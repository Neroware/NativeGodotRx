#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::pluck_(const Variant& key) {
    observable_op_t pluck = OP(source) {
        auto mapper = [=] (const Dictionary& dict) {
            return dict[key];
        };
        return source->pipe(map(mapper));
    };

    return pluck;
}

observable_op_t ops::pluck_attr_(const String& prop) {
    observable_op_t pluck_attr = OP(source) {
        auto mapper = [=] (const Variant& v) {
            bool r_valid;
            return v.get_named(prop, r_valid);
        };
        return source->pipe(map(mapper));
    };

    return pluck_attr;
}

} // END namespace rx::observable::op

