#ifndef RX_OBSERVABLE_REPEAT_H
#define RX_OBSERVABLE_REPEAT_H

#include "observable/observable.h"

using namespace rx::scheduler;

namespace rx::observable {

static std::shared_ptr<Observable> repeat_() {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        throw NotImplementedException();
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_REPEAT_H