#include "wrapper/observable.h"

namespace rx::wrappers {

void RxObservable::_bind_methods() {
    RX_WRAPPER_CAST_BINDS(RxObservable)
}

} // END namespace rx::wrappers