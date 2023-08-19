#ifndef RX_OBSERVABLE_NEVER_H
#define RX_OBSERVABLE_NEVER_H

#include "observable/observable.h"

using namespace rx::scheduler;

namespace rx::observable {

static std::shared_ptr<Observable> never_() {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        return std::make_shared<Disposable>();
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_NEVER_H