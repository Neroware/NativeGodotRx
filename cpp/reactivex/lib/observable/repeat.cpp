#include "observable/definitions.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::repeat_(const Variant& value, int64_t repeat_count) {
    auto xs = return_value(value);
    return xs->pipe(op::repeat(repeat_count));
}

} // END namespace rx::observable