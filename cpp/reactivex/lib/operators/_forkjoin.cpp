#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::fork_join_(const rx_observable_list_t& args) {

    observable_op_t fork_join = OP(source) {
        observable_list_t sources(args.begin(), args.end());
        sources.push_front(source);
        return rx::observable::fork_join(sources); 
    };

    return fork_join;
}

observable_op_t ops::fork_join_(const rx_observable_t& others...) {

    observable_op_t fork_join = OP(source) {
        return rx::observable::fork_join(source, others);
    };

    return fork_join;
}

} // END namespace rx::observable::op
