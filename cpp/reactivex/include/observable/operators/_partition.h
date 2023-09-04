#ifndef RX_OBSERVABLE_OPERATORS_PARTITION_H
#define RX_OBSERVABLE_OPERATORS_PARTITION_H

#include "observable/observable.h"
#include "observable/operators/_publish.h"
#include "observable/operators/connectable/_refcount.h"
#include "observable/operators/_filter.h"
#include "typing.h"

namespace rx::observable::op {

static partition_op_t partition_(const predicate_t<Variant>& predicate) {

    partition_op_t partition = [=](const rx_observable_t& source) -> observable_vec_t {
        rx_observable_t published = source->pipe(
            publish_(),
            ref_count_()
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

static partition_op_t partition_indexed_(const predicate_indexed_t<Variant>& predicate) {

    partition_op_t partition_indexed = [=](const rx_observable_t& source) -> observable_vec_t {
        rx_observable_t published = source->pipe(
            publish_(),
            ref_count_()
        );

        auto not_predicate = [predicate](const Variant& x, uint64_t index) -> bool {
            return !predicate(x, index);
        };

        return {
            published->pipe(filter_indexed_(predicate)),
            published->pipe(filter_indexed_(not_predicate))
        };
    };

    return partition_indexed;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_PARTITION_H
