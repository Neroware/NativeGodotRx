#include "observable/definitions.h"

#include "disposable/compositedisposable.h"
#include "scheduler/scenetreetimeoutscheduler.h"

namespace rx::observable::op {

static disposable_t _schedule(const scheduler_t& s, const time_delta_t& t, const scheduled_action_t& action) {
    return s->schedule_relative(t, action);
}
static disposable_t _schedule(const scheduler_t& s, const time_point_t& t, const scheduled_action_t& action) {
    return s->schedule_absolute(t, action);
}

template<typename TimeT>
static observable_op_t skip_until_with_time_template_(const TimeT& start_time, const scheduler_t& scheduler) {

    auto skip_until_with_time = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();
            auto open = std::make_shared<bool>(false);

            on_next_t on_next = [=](const Variant& x) {
                if (*open) {
                    observer->on_next(x);
                }
            };

            auto subscription = source->subscribe(
                on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler_
            );

            auto action = ACTION(scheduler__, state) {
                *open = true;
                return nullptr;
            };

            auto disp = _schedule(_scheduler, start_time, action);
            return std::make_shared<CompositeDisposable>(disp, subscription);
        };

        return Observable::get(subscribe);
    };

    return skip_until_with_time;
}

observable_op_t ops::skip_until_with_time_(const time_delta_t& start_time, const scheduler_t& scheduler) {
    return skip_until_with_time_template_(start_time, scheduler);
}
observable_op_t ops::skip_until_with_time_(const time_point_t& start_time, const scheduler_t& scheduler) {
    return skip_until_with_time_template_(start_time, scheduler);
}

} // END namespace rx::observable::op
