#ifndef RX_OBSERVABLE_OPERATORS_PLUCK_H
#define RX_OBSERVABLE_OPERATORS_PLUCK_H

#include "observable/observable.h"
#include "observable/operators/_map.h"

namespace rx::observable::op {

static observable_op_t pluck_(const Variant& key) {
    observable_op_t pluck = OP(source) {
        auto mapper = [=] (const Dictionary& dict) {
            return dict[key];
        };
        return source->pipe(map_(mapper));
    };

    return pluck;
}

static observable_op_t pluck_attr_(const String& prop) {
    observable_op_t pluck_attr = OP(source) {
        auto mapper = [=] (const Variant& v) {
            bool r_valid;
            return v.get_named(prop, r_valid);
        };
        return source->pipe(map_(mapper));
    };

    return pluck_attr;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_PLUCK_H
