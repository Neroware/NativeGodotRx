#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::group_by_(
    const mapper_t<Variant, Variant>& key_mapper,
    const mapper_t<Variant, Variant>& element_mapper,
    const mapper_t<rx_subject_t>& subject_mapper
) {

    mapper_t<rx_observable_t, rx_grouped_observable_t> duration_mapper = [](const rx_grouped_observable_t& group) -> rx_observable_t {
        return never();
    };

    return group_by_until(key_mapper, duration_mapper, element_mapper, subject_mapper);
}

} // END namespace rx::observable::op
