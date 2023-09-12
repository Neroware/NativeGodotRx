#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::on_error_resume_next_(const rx_observable_t& second) {
    observable_op_t on_error_resume_next_with = OP(source) {
        return rx::observable::on_error_resume_next(source, second);
    };
    
    return on_error_resume_next_with;
}

} // END namespace rx::observable::op
