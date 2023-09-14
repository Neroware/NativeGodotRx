#include "observable/definitions.h"

namespace rx::observable {

rx_observable_t obs::interval_(const time_delta_t& period, const scheduler_t& scheduler) {
    return timer(period, period, scheduler);
}

} // END namespace rx::observable