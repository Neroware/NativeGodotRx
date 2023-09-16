#include "observable/definitions.h"

#include "scheduler/scenetreetimeoutscheduler.h"
#include "disposable/singleassignmentdisposable.h"

#include "disposable/compositedisposable.h"
#include "disposable/serialdisposable.h"


namespace rx::observable::op {

inline static disposable_t _schedule(const scheduler_t& scheduler, const time_delta_t& duetime, const scheduled_action_t& action) {
    return scheduler->schedule_relative(duetime, action);
}
inline static disposable_t _schedule(const scheduler_t& scheduler, const time_point_t& duetime, const scheduled_action_t& action) {
    return scheduler->schedule_absolute(duetime, action);
}

template<typename AbsoluteOrRelativeTime>
static observable_op_t _timeout(const AbsoluteOrRelativeTime& duetime, const rx_observable_t& other, const scheduler_t& scheduler) {

    if (!other) throw rx_exception("Timeout!");
    auto obs = other;

    observable_op_t timeout = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            auto switched = std::make_shared<bool>(false);
            auto _id = std::make_shared<int64_t>(0);

            auto original = std::make_shared<SingleAssignmentDisposable>();
            auto subscription = std::make_shared<SerialDisposable>();
            auto timer = std::make_shared<SerialDisposable>();
            subscription->set_disposable(original);

            auto create_timer = [=]() {
                int64_t my_id = *_id;

                auto action = ACTION(scheduler, state) {
                    *switched = *_id == my_id;
                    bool timer_wins = *switched;
                    if (timer_wins) {
                        subscription->set_disposable(obs->subscribe(observer, scheduler));
                    }
                    return nullptr;
                };

                timer->set_disposable(_schedule(_scheduler, duetime, action));
            };

            create_timer();

            auto on_next = [=](const Variant& value) {
                bool send_wins = !*switched;
                if (send_wins) {
                    ++(*_id);
                    observer->on_next(value);
                    create_timer();
                }
            };

            auto on_error = [=](const std::exception_ptr& error) {
                bool on_error_wins = !*switched;
                if (on_error_wins) {
                    ++(*_id);
                    observer->on_error(error);
                }
            };

            auto on_completed = [=]() {
                bool on_completed_wins = !*switched;
                if (on_completed_wins) {
                    ++(*_id);
                    observer->on_completed();
                }
            };

            original->set_disposable(source->subscribe(on_next, on_error, on_completed, scheduler_));
            return std::make_shared<CompositeDisposable>(subscription, timer);
        };

        return Observable::get(subscribe);
    };

    return timeout;
}

observable_op_t ops::timeout_(const time_delta_t& duetime, const rx_observable_t& other, const scheduler_t& scheduler) {
    return _timeout(duetime, other, scheduler);
}
observable_op_t ops::timeout_(const time_point_t& duetime, const rx_observable_t& other, const scheduler_t& scheduler) {
    return _timeout(duetime, other, scheduler);
}

} // END namespace rx::observable::op
