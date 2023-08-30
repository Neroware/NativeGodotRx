#ifndef RX_OBSERVABLE_OPERATORS_SCAN_H
#define RX_OBSERVABLE_OPERATORS_SCAN_H

#include "observable/observable.h"

#include "observable/operators/_map.h"
#include "observable/defer.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t scan_(const accumulator_t<Variant, Variant>& accumulator, const Variant& seed = NotSet::value()) {

    bool has_seed = IS_SET(seed);

    observable_op_t scan = OP(source) {

        observable_factory_t factory = [=](const scheduler_t& scheduler) -> std::shared_ptr<Observable> {
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

            return source->pipe(map_(projection));
        };

        return defer_(factory);
    };

    return scan;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_SCAN_H
