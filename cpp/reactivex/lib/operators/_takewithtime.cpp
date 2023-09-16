#include "observable/definitions.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::take_with_time_(const time_delta_t& duration, const scheduler_t& scheduler) {

    observable_op_t take_with_time = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            auto action = ACTION(scheduler__, state) {
                observer->on_completed();
                return nullptr;
            };

            auto disp = _scheduler->schedule_relative(duration, action);
            return std::make_shared<CompositeDisposable>(
                disp, source->subscribe(observer, scheduler)
            );
        };

        return Observable::get(subscribe);
    };

    return take_with_time;
}

} // END namespace rx::observable::op
