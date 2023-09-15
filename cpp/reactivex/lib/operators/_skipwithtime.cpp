#include "observable/definitions.h"

#include "scheduler/scenetreetimeoutscheduler.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::skip_with_time_(const time_delta_t& duration, const scheduler_t& scheduler) {

    observable_op_t skip_with_time = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler : SceneTreeTimeoutScheduler::singleton());
            auto open = std::make_shared<bool>(false);

            auto action = ACTION(scheduler__, state) {
                *open = true; return nullptr;
            };

            auto t = _scheduler->schedule_relative(duration, action);

            auto on_next = [=](const Variant& x) {
                if (*open) {
                    observer->on_next(x);
                }
            };

            auto d = source->subscribe(
                on_next, ONERROR_FWD(observer), 
                ONCOMPLETED_FWD(observer), scheduler_
            );

            return std::make_shared<CompositeDisposable>(t, d);
        };

        return Observable::get(subscribe);
    };

    return skip_with_time;
}

} // END namespace rx::observable::op
