#include "observable/definitions.h"

#include "subject/subject.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/refcountdisposable.h"

#include "scheduler/scenetreetimeoutscheduler.h"


using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::window_with_time_(
    const time_delta_t& timespan,
    const time_delta_t& timeshift,
    const scheduler_t& scheduler
) {

    observable_op_t window_with_time = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

            auto timer_d = std::make_shared<SerialDisposable>();
            std::shared_ptr<time_delta_t> next_shift = std::make_shared<time_delta_t>(timeshift);
            std::shared_ptr<time_delta_t> next_span = std::make_shared<time_delta_t>(timespan);
            std::shared_ptr<time_delta_t> total_time = std::make_shared<time_delta_t>(DELTA_ZERO);

            auto queue = std::make_shared<std::list<rx_subject_t>>();

            auto group_disposable = std::make_shared<CompositeDisposable>();
            group_disposable->add(timer_d);
            auto ref_count_disposable = RefCountDisposable::get(group_disposable);

            auto create_timer = [=](const auto& _create_timer) -> void {
                auto m = std::make_shared<SingleAssignmentDisposable>();
                timer_d->set_disposable(m);
                bool is_span = false;
                bool is_shift = false;

                if (*next_span == *next_shift) {
                    is_span = true;
                    is_shift = true;
                } else if (*next_span < *next_shift) {
                    is_span = true;
                } else {
                    is_shift = true;
                }

                auto new_total_time = is_span ? *next_span : *next_shift;

                auto ts = new_total_time - *total_time;
                *total_time = new_total_time;

                if (is_span) {
                    *next_span += timeshift;
                }

                if (is_shift) {
                    *next_shift += timeshift;
                }

                auto action = ACTION(scheduler, state) {
                    std::lock_guard<RLock> guard(source->lock);

                    rx_subject_t s;

                    if (is_shift) {
                        s = Subject::get();
                        queue->push_back(s);
                        observer->on_next(add_ref(s, ref_count_disposable));
                    }

                    if (is_span) {
                        s = queue->front();
                        queue->pop_front();
                        s->on_completed();
                    }

                    _create_timer(_create_timer);
                    return nullptr;
                };
            };

            queue->push_back(Subject::get());
            observer->on_next(add_ref(queue->front(), ref_count_disposable));
            create_timer(create_timer);

            auto on_next = [=](const Variant& x) {
                std::lock_guard<RLock> guard(source->lock);
                for (const auto& s : *queue) {
                    s->on_next(x);
                }
            };

            auto on_error = [=](const std::exception_ptr& exception) {
                std::lock_guard<RLock> guard(source->lock);
                for (const auto& s : *queue) {
                    s->on_error(exception);
                }
                observer->on_error(exception);
            };

            auto on_completed = [=]() {
                std::lock_guard<RLock> guard(source->lock);
                for (const auto& s : *queue) {
                    s->on_completed();
                }
                observer->on_completed();
            };

            group_disposable->add(
                source->subscribe(on_next, on_error, on_completed, scheduler_)
            );
            return ref_count_disposable;
        };

        return Observable::get(subscribe);
    };

    return window_with_time;
}

} // END namespace rx::observable::op
