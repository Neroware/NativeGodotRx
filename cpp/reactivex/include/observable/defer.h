#ifndef RX_OBSERVABLE_DEFER_H
#define RX_OBSERVABLE_DEFER_H

#include "observable/observable.h"
#include "observable/throw.h"

#include "scheduler/immediatescheduler.h"

namespace rx::observable {

static std::shared_ptr<Observable> defer_(const observable_factory_t& factory) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        std::shared_ptr<Observable> result;
        try {
            result = factory(scheduler_ ? scheduler_ : ImmediateScheduler::singleton());
        }
        catch(...) {
            return throw_(std::current_exception())->subscribe(observer);
        }

        return result->subscribe(observer, scheduler_);
    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_DEFER_H