#include "observable/definitions.h"

#include "subject/subject.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::publish_(const mapper_t<rx_observable_t, rx_observable_t>& mapper) {
    
    if (mapper) {
        rx_subject_factory_t factory = [=](const scheduler_t& scheduler = nullptr) -> rx_subject_t {
            return Subject::get();
        };

        return multicast(factory, mapper);
    }

    rx_subject_t subject = Subject::get();
    return multicast(subject);
}

observable_op_t ops::share_() {
    return pipe::compose(
        publish(),
        ref_count()
    );
}

} // END namespace rx::observable::op

