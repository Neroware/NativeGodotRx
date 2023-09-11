#include "observable/definitions.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::never_() {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_) {
        return std::make_shared<Disposable>();
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable
