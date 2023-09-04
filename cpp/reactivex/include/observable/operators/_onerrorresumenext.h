#ifndef RX_OBSERVABLE_OPERATORS_ONERRORRESUMENEXT_H
#define RX_OBSERVABLE_OPERATORS_ONERRORRESUMENEXT_H

#include "observable/observable.h"
#include "observable/onerrorresumenext.h"


namespace rx::observable::op {

static observable_op_t on_error_resume_next_with_(const rx_observable_t& second) {
    observable_op_t on_error_resume_next_with = OP(source) {
        return on_error_resume_next_(source, second);
    };
    
    return on_error_resume_next_with;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ONERRORRESUMENEXT_H