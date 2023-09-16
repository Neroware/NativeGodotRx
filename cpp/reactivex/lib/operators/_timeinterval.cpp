#include "observable/definitions.h"

#include "scheduler/scenetreetimeoutscheduler.h"

namespace rx::observable::op {

observable_op_t ops::time_interval_(const scheduler_t& scheduler) {

    observable_op_t time_interval = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler : SceneTreeTimeoutScheduler::singleton());
            auto last = std::make_shared<time_point_t>(_scheduler->now());

            auto mapper = [=](const Variant& value) -> Ref<RxTimeInterval> {
                time_point_t now = _scheduler->now();
                time_delta_t span = now - *last;
                *last = now;
                return memnew(RxTimeInterval(memnew(RelativeTime(span)), value));
            };

            return source->pipe(map(mapper))->subscribe(
                observer, _scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return time_interval;
}

} // END namespace rx::observable::op
