#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::buffer_with_time_or_count_(
    const time_delta_t& timespan, 
    int64_t count, 
    const scheduler_t& scheduler
) {
    return pipe::compose(
        window_with_time_or_count(timespan, count, scheduler),
        flat_map(to_list())
    );
}

}