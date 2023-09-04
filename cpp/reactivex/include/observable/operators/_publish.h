#ifndef RX_OBSERVABLE_OPERATORS_PUBLISH_H
#define RX_OBSERVABLE_OPERATORS_PUBLISH_H

#include "observable/observable.h"

#include "observable/operators/_multicast.h"
#include "observable/operators/connectable/_refcount.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t publish_(const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr) {
    
    if (mapper) {
        rx_subject_factory_t factory = [=](const scheduler_t& scheduler = nullptr) -> rx_subject_t {
            return Subject::get();
        };

        return multicast_(factory, mapper);
    }

    rx_subject_t subject = Subject::get();
    return multicast_(subject);
}

static observable_op_t share_() {
    return pipe::compose(
        publish_(),
        ref_count_()
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_PUBLISH_H
