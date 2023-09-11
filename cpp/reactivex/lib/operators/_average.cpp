#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::average_(const mapper_t<double, Variant>& key_mapper) {

    observable_op_t average = OP(source) {

        auto key_mapper_ = key_mapper ? key_mapper : [](const Variant& x) -> double {
            return x;
        };

        accumulator_t<Array, double> accumulator = [](const Array& prev, double cur) -> Array {
            Array _prev = prev; double sum = prev[0]; uint64_t count = prev[1];
            _prev[0] = sum + cur; _prev[1] = count + 1;
            return _prev;
        };

        mapper_t<double, Array> mapper = [](const Array& s) -> double {
            uint64_t count = s[1];
            double sum = s[0];
            if (count == 0) {
                throw SequenceContainsNoElementsException();
            }
            return sum / static_cast<double>(count);
        };

        Array seed = Array::make(0.0, 0);

        auto ret = source->pipe(
            map(key_mapper_),
            scan(accumulator, seed),
            last(),
            map(mapper)
        );
        return ret;
    };

    return average;
}

} // END namespace rx::observable::op
