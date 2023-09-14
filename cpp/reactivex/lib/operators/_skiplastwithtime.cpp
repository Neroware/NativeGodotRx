#include "observable/definitions.h"
#include "scheduler/scenetreetimeoutscheduler.h"

using namespace std::chrono;
using namespace rx::scheduler;

namespace rx::observable::op {

observable_op_t ops::skip_last_with_time_(const time_point_t& duration, const scheduler_t& scheduler) {
    return ops::skip_last_with_time_(Scheduler::to_timedelta(duration), scheduler);
}

observable_op_t ops::skip_last_with_time_(const time_delta_t& duration, const scheduler_t& scheduler) {

    auto skip_last_with_time = OP(source) {

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto scheduler_ = scheduler ? scheduler : SceneTreeTimeoutScheduler::singleton();
            auto q = std::make_shared<std::list<std::pair<Variant, time_point_t>>>();

            on_next_t on_next = [=](const Variant& value) {
                auto now = scheduler_->now();
                q->push_back({value, now});
                while (!q->empty() && now - q->front().second >= duration) {
                    observer->on_next(q->front().first);
                    q->pop_front();
                }
            };

            on_completed_t on_completed = [=]() {
                auto now = scheduler_->now();
                while (!q->empty() && now - q->front().second >= duration) {
                    observer->on_next(q->front().first);
                    q->pop_front();
                }
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), on_completed, scheduler_
            );
        };

        return Observable::get(subscribe);
    };

    return skip_last_with_time;
}

} // END namespace rx::observable::op
