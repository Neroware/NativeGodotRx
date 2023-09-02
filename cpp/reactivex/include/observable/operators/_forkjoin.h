#ifndef RX_OBSERVABLE_OPERATORS_FORKJOIN_H
#define RX_OBSERVABLE_OPERATORS_FORKJOIN_H

#include "observable/observable.h"
#include "observable/forkjoin.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

template<typename IterableT>
static observable_op_t fork_join_(const IterableT& args) {
    observable_vec_t sources;
    sources.push_back(nullptr);
    for (auto& obs : args) sources.push_back(obs);

    observable_op_t fork_join = OP(source) {
        
        observable_vec_t _sources(sources);
        _sources[0] = source;

        return rx::observable::fork_join_(_sources); 
    };

    return fork_join;
}
template <typename... Args>
static observable_op_t fork_join_(const Args&... others) {

    observable_op_t fork_join = OP(source) {
        return rx::observable::fork_join_(source, others...);
    };

    return fork_join;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_FORKJOIN_H
