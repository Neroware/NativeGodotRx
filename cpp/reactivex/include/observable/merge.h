#ifndef RX_OBSERVABLE_MERGE_H
#define RX_OBSERVABLE_MERGE_H

#include "observable/observable.h"

#include "observable/operators/_merge.h"

using namespace rx::scheduler;

namespace rx::observable {

template<typename IterableT>
static rx_observable_t merge_(const IterableT& sources) {
    return op::_merge(sources);
}
template<typename... Args>
static rx_observable_t merge_(const Args&... sources) {
    observable_vec_t _sources = {sources...};
    return op::_merge(_sources);
}


} // END namespace rx::observable

#endif // RX_OBSERVABLE_MERGE_H