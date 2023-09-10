#include "observable/definitions.h"

#include "scheduler/immediatescheduler.h"

namespace rx::observable {

rx_observable_t obs::defer_(const observable_factory_t& factory) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        rx_observable_t result;
        try {
            result = factory(scheduler_ ? scheduler_ : ImmediateScheduler::singleton());
        }
        catch(...) {
            return throw_error(std::current_exception())->subscribe(observer);
        }

        return result->subscribe(observer, scheduler_);
    };

    return Observable::get(subscribe);
}

}