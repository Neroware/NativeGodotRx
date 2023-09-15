#include "observable/definitions.h"

namespace rx::observable::op {

static rx_observable_t _flat_map_internal(
    const rx_observable_t& source,
    const mapper_t<Variant, Variant>& mapper = nullptr,
    const mapper_indexed_t<Variant, Variant>& mapper_indexed = nullptr
) {
    auto projection = [=](const Variant& x, int i) -> rx_observable_t {
        Variant mapper_result = mapper ? mapper(x) : (mapper_indexed ? mapper_indexed(x, i) : basic::identity<const Variant&>(x));

        rx_observable_t result = nullptr;
        if (rx_observable_t _mapper_result = mapper_result) {
            result = _mapper_result;
        }
        else {
            result = from(mapper_result);
        }
        return result;
    };

    return source->pipe(
        map_indexed(projection),
        merge_all()
    );
}

observable_op_t ops::flat_map_(const mapper_t<rx_observable_t, Variant>& mapper) {
    observable_op_t flat_map = OP(source) {
        if (mapper) {
            return _flat_map_internal(source, mapper);
        }
        else {
            return _flat_map_internal(source, [](const Variant&) { return VNULL; });
        }
    };

    return flat_map;
}

observable_op_t ops::flat_map_indexed_(const mapper_indexed_t<rx_observable_t, Variant>& mapper) {
    observable_op_t flat_map_indexed = OP(source) {
        if (mapper) {
            return _flat_map_internal(source, nullptr, mapper);
        }
        else {
            return _flat_map_internal(source, [](const Variant&) { return VNULL; });
        }
    };

    return flat_map_indexed;
}

observable_op_t ops::flat_map_latest_(const mapper_t<rx_observable_t, Variant>& mapper) {
    observable_op_t flat_map_latest = OP(source) {
        return source->pipe(
            map(mapper),
            switch_latest()
        );
    };

    return flat_map_latest;
}

}