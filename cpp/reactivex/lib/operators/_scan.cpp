#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed) {

    bool has_seed = IS_SET(seed);

    observable_op_t scan = OP(source) {

        observable_factory_t factory = [=](const scheduler_t& scheduler) -> rx_observable_t {
            auto has_accumulation = std::make_shared<bool>(false);
            auto accumulation = std::make_shared<Variant>();

            auto projection = [=](const Variant& x) -> Variant {
                if (*has_accumulation) {
                    *accumulation = accumulator(*accumulation, x);
                } else {
                    *accumulation = has_seed ? accumulator(seed, x) : x;
                    *has_accumulation = true;
                }
                return *accumulation;
            };

            return source->pipe(map(projection));
        };

        return defer(factory);
    };

    return scan;
}

} // END namespace rx::observable::op
