#ifndef RX_OBSERVABLE_OPERATORS_GROUP_BY_H
#define RX_OBSERVABLE_OPERATORS_GROUP_BY_H

#include "observable/observable.h"

#include "observable/never.h"
#include "observable/operators/_groupbyuntil.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t group_by_(
    const mapper_t<Variant, Variant>& key_mapper,
    const mapper_t<Variant, Variant>& element_mapper = nullptr,
    const mapper_t<rx_subject_t>& subject_mapper = nullptr
) {

    mapper_t<rx_observable_t, grouped_observable_t> duration_mapper = [](const grouped_observable_t& group) -> rx_observable_t {
        return never_();
    };

    return group_by_until_(key_mapper, duration_mapper, element_mapper, subject_mapper);
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_GROUP_BY_H
