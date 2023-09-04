#ifndef RX_OBSERVABLE_OPERATORS_PUBLISHVALUE_H
#define RX_OBSERVABLE_OPERATORS_PUBLISHVALUE_H

#include "observable/observable.h"
#include "subject/behaviorsubject.h"
#include "observable/operators/_multicast.h"

using namespace rx::subject;
using namespace rx::observer;

namespace rx::observable::op {

static observable_op_t publish_value_(
    const Variant& initial_value, 
    const mapper_t<rx_observable_t, rx_observable_t>& mapper = nullptr
) {
    if (mapper) {
        auto subject_factory = [=] (const scheduler_t& scheduler) {
            return BehaviorSubject::get(initial_value);
        };
        return multicast_(subject_factory, mapper);
    }

    auto subject = BehaviorSubject::get(initial_value);
    return multicast_(subject);
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_PUBLISHVALUE_H
