#include "observable/definitions.h"

#include "disposable/multiassignmentdisposable.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::range_(int64_t start, int64_t stop, int64_t step, const scheduler_t& scheduler) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

        auto current = std::make_shared<int64_t>(start);
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();
        auto sd = std::make_shared<MultiAssignmentDisposable>();

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            if (*current < stop) {
                observer->on_next(*current);
                *current += step;
                sd->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(_action), *current));
            }
            else {
                observer->on_completed();
            }
            return nullptr;
        };

        sd->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(action), *current));
        return sd;
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable