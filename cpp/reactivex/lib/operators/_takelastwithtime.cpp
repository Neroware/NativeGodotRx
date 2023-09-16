#include "observable/definitions.h"

#include "scheduler/scenetreetimeoutscheduler.h"

namespace rx::observable::op {

observable_op_t ops::take_last_with_time_(const time_delta_t& duration, const scheduler_t& scheduler) {

    observable_op_t take_last_with_time = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

            
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());
            time_delta_t _duration = duration;
            auto q = std::make_shared<std::list<std::pair<time_point_t, Variant>>>();

            on_next_t on_next = [=](const Variant& x) {
                auto now = _scheduler->now();
                q->push_back(std::make_pair(now, x));
                while (!q->empty() && now - q->front().first >= _duration) {
                    q->pop_front();
                }
            };

            on_completed_t on_completed = [=]() {
                auto now = _scheduler->now();
                while (!q->empty()) {
                    auto& next = q->front();
                    if (now - next.first <= _duration) {
                        observer->on_next(next.second);
                    }
                    q->pop_front();
                }
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler_
            );
        };

        return Observable::get(subscribe);
    };

    return take_last_with_time;
}

} // END namespace rx::observable::op
