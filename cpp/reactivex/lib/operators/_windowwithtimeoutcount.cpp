#include "observable/definitions.h"

#include "subject/subject.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/refcountdisposable.h"

#include "scheduler/scenetreetimeoutscheduler.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::window_with_time_or_count_(
    const time_delta_t& timespan,
    int64_t count,
    const scheduler_t& scheduler
) {
    observable_op_t window_with_time_or_count = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : TimeoutScheduler::singleton());

            auto n = std::make_shared<int64_t>(0);
            auto s = std::make_shared<rx_subject_t>(Subject::get());
            auto timer_d = std::make_shared<SerialDisposable>();
            auto window_id = std::make_shared<int64_t>(0);
            auto group_disposable = std::make_shared<CompositeDisposable>();
            group_disposable->add(timer_d);
            auto ref_count_disposable = RefCountDisposable::get(group_disposable);

            auto create_timer = [=](int64_t _id, const auto& _create_timer) -> void {
                auto m = std::make_shared<SingleAssignmentDisposable>();
                timer_d->set_disposable(m);

                auto action = ACTION(scheduler, state) {
                    if (_id != *window_id) {
                        return nullptr;
                    }

                    *n = 0;
                    *window_id += 1;
                    int64_t new_id = *window_id;
                    (*s)->on_completed();
                    *s = Subject::get();
                    observer->on_next(add_ref(*s, ref_count_disposable));
                    _create_timer(new_id, _create_timer);
                    return nullptr;
                };

                m->set_disposable(_scheduler->schedule_relative(timespan, action));
            };

            observer->on_next(add_ref(*s, ref_count_disposable));
            create_timer(0, create_timer);

            auto on_next = [=](const Variant& x) {
                bool new_window = false;
                int64_t new_id = 0;

                (*s)->on_next(x);
                *n += 1;
                if (*n == count) {
                    new_window = true;
                    *n = 0;
                    *window_id += 1;
                    new_id = *window_id;
                    (*s)->on_completed();
                    *s = Subject::get();
                    observer->on_next(add_ref(*s, ref_count_disposable));
                }

                if (new_window) {
                    create_timer(new_id, create_timer);
                }
            };

            auto on_error = [=](const std::exception_ptr& e) {
                (*s)->on_error(e);
                observer->on_error(e);
            };

            auto on_completed = [=]() {
                (*s)->on_completed();
                observer->on_completed();
            };

            group_disposable->add(
                source->subscribe(on_next, on_error, on_completed, scheduler_)
            );
            return ref_count_disposable;
        };

        return Observable::get(subscribe);
    };

    return window_with_time_or_count;
}

} // END namespace rx::observable::op
