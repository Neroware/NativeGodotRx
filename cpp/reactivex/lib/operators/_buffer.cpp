#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::buffer_(const rx_observable_t& boundaries) {
    return pipe::compose(
        window(boundaries),
        flat_map(to_list())
    );
}

observable_op_t ops::buffer_when_(const mapper_t<rx_observable_t>& closing_mapper) {
    return pipe::compose(
        window_when(closing_mapper),
        flat_map(to_list())
    );
}

observable_op_t ops::buffer_toggle_(const rx_observable_t& openings, const mapper_t<rx_observable_t, Variant>& closing_mapper) {
    return pipe::compose(
        window_toggle(openings, closing_mapper),
        flat_map(to_list())
    );
}

observable_op_t ops::buffer_with_count_(int64_t count, int64_t skip_) {

    observable_op_t buffer_with_count = OP(source) {
        int64_t skip = skip_ < 0 ? count : skip_;

        auto mapper = [](const rx_observable_t& value) -> rx_observable_t {
            return value->pipe(
                to_list()
            );
        };

        auto predicate = [](const Array& value) -> bool {
            return value.size() > 0;
        };

        return source->pipe(
            window_with_count(count, skip),
            flat_map(mapper),
            filter(predicate)
        );
    };

    return buffer_with_count;
}


} // END namespace rx::observable::op