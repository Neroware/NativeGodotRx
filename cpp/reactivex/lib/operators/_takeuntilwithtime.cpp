#include "observable/definitions.h"

#include "scheduler/scenetreetimeoutscheduler.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::take_until_with_time_(const time_delta_t& end_time, const scheduler_t& scheduler) {

    observable_op_t take_until_with_time = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            auto action = ACTION(scheduler, state) {
                observer->on_completed();
                return nullptr;
            };

            auto task = _scheduler->schedule_relative(end_time, action);

            return std::make_shared<CompositeDisposable>(
                task, source->subscribe(observer, scheduler_)
            );
        };

        return Observable::get(subscribe);
    };

    return take_until_with_time;
}
observable_op_t ops::take_until_with_time_(const time_point_t& end_time, const scheduler_t& scheduler) {

    observable_op_t take_until_with_time = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            auto action = ACTION(scheduler, state) {
                observer->on_completed();
                return nullptr;
            };

            auto task = _scheduler->schedule_absolute(end_time, action);

            return std::make_shared<CompositeDisposable>(
                task, source->subscribe(observer, scheduler_)
            );
        };

        return Observable::get(subscribe);
    };

    return take_until_with_time;
}

} // END namespace rx::observable::op
