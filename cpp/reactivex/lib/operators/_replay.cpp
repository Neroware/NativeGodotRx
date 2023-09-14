#include "observable/definitions.h"

#include "subject/replaysubject.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::replay_(
    const mapper_t<rx_observable_t, rx_observable_t>& mapper,
    uint64_t buffer_size,
    const time_delta_t& window,
    const scheduler_t scheduler
) {

    if (mapper) {
            
        rx_subject_factory_t subject_factory = [=](const scheduler_t& scheduler) -> rx_subject_t {
            return ReplaySubject::get(buffer_size, window, scheduler);
        };

        return multicast(subject_factory, mapper);
    }

    auto rs = ReplaySubject::get(buffer_size, window, scheduler);
    return multicast(rs);
}

} // END namespace rx::observable::op
