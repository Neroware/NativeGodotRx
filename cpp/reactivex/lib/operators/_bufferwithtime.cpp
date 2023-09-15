#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::buffer_with_time_(
    const time_delta_t& timespan, 
    const time_delta_t& timeshift_, 
    const scheduler_t& scheduler
) {
    auto timeshift = timeshift_ == DELTA_ZERO ? timespan : timeshift_;

    return pipe::compose(
        window_with_time(timespan, timeshift, scheduler),
        flat_map(to_list())
    );
}

}