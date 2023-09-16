#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "scheduler/scenetreetimeoutscheduler.h"

namespace rx::observable::op {

observable_op_t ops::throttle_first_(const time_delta_t& window_duration, const scheduler_t& scheduler) {

    observable_op_t throttle_first = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            time_delta_t duration = window_duration;
            if (duration <= DELTA_ZERO) {
                throw BadArgumentException("window_duration cannot be less or equal zero.");
            }
            auto last_on_next = std::make_shared<time_point_t>(UTC_ZERO);
            auto has_last_on_next = std::make_shared<bool>(false);

            auto on_next = [=](const Variant& x) {
                bool emit = false;
                time_point_t now = _scheduler->now();

                {
                    std::lock_guard<RLock> lock(source->lock);
                    if (!*has_last_on_next || now - *last_on_next >= duration) {
                        *last_on_next = now;
                        *has_last_on_next = true;
                        emit = true;
                    }
                }

                if (emit) {
                    observer->on_next(x);
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), _scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return throttle_first;
}

} // END namespace rx::observable::op
