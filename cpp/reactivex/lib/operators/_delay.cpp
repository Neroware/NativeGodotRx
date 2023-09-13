#include "observable/definitions.h"

#include "disposable/multiassignmentdisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

namespace rx::observable::op {

rx_observable_t ops::observable_delay_timespan(const rx_observable_t& source, const time_delta_t& duetime, const scheduler_t& scheduler) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        scheduler_t _scheduler = scheduler ? scheduler : (scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton());

        auto cancelable = std::make_shared<SerialDisposable>();
        auto exception = std::make_shared<error_t>();
        auto active = std::make_shared<bool>(false);
        auto running = std::make_shared<bool>(false);
        auto queue = std::make_shared<std::list<timestamp_t<notification_t>>>();

        on_next_t on_next = [=](const timestamp_t<notification_t>& notification) {
            bool should_run = false;

            {
                std::lock_guard<RLock> guard(source->lock);

                if (auto n = std::dynamic_pointer_cast<NotificationOnError>(notification.value)) {
                    queue->clear();
                    queue->push_back(notification);
                    *exception = n->exception;
                    should_run = !(*running);
                } else {
                    queue->push_back(timestamp_t<notification_t>(
                        notification.timestamp + duetime,
                        notification.value
                    ));
                    should_run = !(*active);
                    *active = true;
                }
            }

            if (should_run) {
                if (*exception) {
                    observer->on_error(*exception);
                } else {
                    auto mad = std::make_shared<MultiAssignmentDisposable>();
                    cancelable->set_disposable(mad);

                    auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
                        if (*exception) {
                            return nullptr;
                        }

                        error_t ex = nullptr;
                        bool should_continue;
                        time_delta_t recurse_duetime;
                        {
                            std::lock_guard<RLock> guard(source->lock);
                            *running = true;

                            while (true) {
                                notification_t result;

                                if (!queue->empty() && queue->front().timestamp <= scheduler->now()) {
                                    result = queue->front().value;
                                    queue->pop_front();
                                }

                                if (result) {
                                    result->accept(observer);
                                }

                                if (!result) {
                                    break;
                                }
                            }

                            should_continue = false;
                            recurse_duetime = DELTA_ZERO;

                            if (!queue->empty()) {
                                should_continue = true;
                                time_delta_t diff = queue->front().timestamp - scheduler->now();
                                recurse_duetime = std::max(DELTA_ZERO, diff);
                            } else {
                                *active = false;
                            }

                            ex = *exception;
                            *running = false;
                        }

                        if (ex) {
                            observer->on_error(ex);
                        } else if (should_continue) {
                            mad->set_disposable(scheduler->schedule_relative(recurse_duetime, RECURSIVE_ACTION_FWD(_action)));
                        }
                        return nullptr;
                    };

                    mad->set_disposable(_scheduler->schedule_relative(duetime, RECURSIVE_ACTION_FWD(action)));
                }
            }
        };

        auto subscription = source->pipe(
            materialize(),
            timestamp()
        )->subscribe(
            on_next, DEFAULT_ON_ERROR,
            DEFAULT_ON_COMPLETED, _scheduler
        );

        return std::make_shared<CompositeDisposable>(subscription, cancelable);
    };

    return Observable::get(subscribe);
};

observable_op_t ops::delay_(const time_delta_t& duetime, const scheduler_t& scheduler) {
    observable_op_t delay = OP(source) {
        return observable_delay_timespan(source, duetime, scheduler);
    };

    return delay;
}

} // END namespace rx::observable::op