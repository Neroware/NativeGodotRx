#include "observable/definitions.h"

namespace rx::observable::op {

partition_op_t ops::partition_(const predicate_t<Variant>& predicate) {

    partition_op_t partition = [=](const rx_observable_t& source) -> observable_vec_t {
        rx_observable_t published = source->pipe(
            publish(),
            ref_count()
        );

        auto not_predicate = [predicate](const Variant& x) -> bool {
            return !predicate(x);
        };

        return {
            published->pipe(filter_(predicate)),
            published->pipe(filter_(not_predicate))
        };
    };

    return partition;
}

partition_op_t ops::partition_indexed_(const predicate_indexed_t<Variant>& predicate) {

    partition_op_t partition_indexed = [=](const rx_observable_t& source) -> observable_vec_t {
        rx_observable_t published = source->pipe(
            publish(),
            ref_count()
        );

        auto not_predicate = [predicate](const Variant& x, uint64_t index) -> bool {
            return !predicate(x, index);
        };

        return {
            published->pipe(filter_indexed(predicate)),
            published->pipe(filter_indexed(not_predicate))
        };
    };

    return partition_indexed;
}

} // END namespace rx::observable::op
