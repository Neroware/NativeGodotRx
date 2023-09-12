#include "observable/definitions.h"
#include "subject/behaviorsubject.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::publish_value_(
    const Variant& initial_value, 
    const mapper_t<rx_observable_t, rx_observable_t>& mapper
) {
    if (mapper) {
        auto subject_factory = [=] (const scheduler_t& scheduler) {
            return BehaviorSubject::get(initial_value);
        };
        return multicast(subject_factory, mapper);
    }

    auto subject = BehaviorSubject::get(initial_value);
    return multicast(subject);
}

} // END namespace rx::observable::op
