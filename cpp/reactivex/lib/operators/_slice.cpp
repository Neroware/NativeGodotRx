#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::slice_(int64_t start, int64_t stop, int64_t step) {

    observable_op_t slice = OP(source) {

        std::vector<observable_op_t> pipeline;

        if (stop >= 0) {
            pipeline.push_back(take(stop));
        }

        if (start > 0) {
            pipeline.push_back(skip(start));
        } else if (start < 0) {
            pipeline.push_back(take_last(-start));
        }

        if (stop < 0) {
            pipeline.push_back(skip_last(-stop));
        }

        if (step > 1) {
            pipeline.push_back(filter_indexed(
                [=](const Variant& x, uint64_t i) { return i % step == 0; }
            ));
        } else if (step < 0) {
            // Reversing events is not supported
            throw BadArgumentException("Negative step not supported.");
        }

        // Sadly unpacking lists in parameter packs in unsupported in C++
        rx_observable_t res = source;
        for (const auto& op : pipeline) {
            res = op(res);
        }
        return res;
    };
    
    return slice;
}

}