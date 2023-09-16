#include "observable/definitions.h"


namespace rx::observable::op {

observable_op_t ops::sum_(const mapper_t<float, Variant>& key_mapper) {
    if (key_mapper) {
        return pipe::compose(map(key_mapper), ops::sum_());
    }

    auto accumulator = [](double prev, double cur) -> double {
        return prev + cur;
    };

    return reduce(accumulator, 0);
}

}