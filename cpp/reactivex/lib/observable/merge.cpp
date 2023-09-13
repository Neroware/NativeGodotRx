#include "observable/definitions.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::merge_(const rx_observable_list_t& sources) {
    return from_iterable(sources.iter())->pipe(op::merge_all());
}
rx_observable_t obs::merge_(const rx_observable_t& sources...) {
    return merge_({sources});
}

} // END namespace rx::observable