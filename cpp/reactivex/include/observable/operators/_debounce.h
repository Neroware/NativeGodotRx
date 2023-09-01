#ifndef RX_OBSERVABLE_OPERATORS_DEBOUNCE_H
#define RX_OBSERVABLE_OPERATORS_DEBOUNCE_H

#include "observable/observable.h"
#include "observable/operators/_debounce.h"

#include "scheduler/scenetreetimeoutscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t debounce_(const time_delta_t& duetime, const scheduler_t& scheduler = nullptr) {

    observable_op_t debounce = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

            scheduler_t _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();

            auto cancelable = std::make_shared<SerialDisposable>();
            auto has_value = std::make_shared<bool>(false);
            auto value = std::make_shared<Variant>();
            auto _id = std::make_shared<int>(0);

            on_next_t on_next = [=](const Variant& x) {
                *has_value = true;
                *value = x;
                ++(*_id);
                int current_id = *_id;

                auto d = std::make_shared<SingleAssignmentDisposable>();
                cancelable->set_disposable(d);

                action_t action = ACTION(scheduler__, state) {
                    if (has_value && *_id == current_id) {
                        observer->on_next(*value);
                    }
                    *has_value = false;
                    return nullptr;
                };

                d->set_disposable(_scheduler->schedule_relative(duetime, action));
            };

            on_error_t on_error = [=](const std::exception_ptr& ex) {
                cancelable->dispose();
                observer->on_error(ex);
                *has_value = false;
                ++(*_id);
            };

            on_completed_t on_completed = [=]() {
                cancelable->dispose();
                if (*has_value) {
                    observer->on_next(*value);
                }
                observer->on_completed();
                *has_value = false;
                ++(*_id);
            };

            auto subscription = source->subscribe(
                on_next, on_error, on_completed, scheduler_
            );
            return std::make_shared<CompositeDisposable>(subscription, cancelable);
        };

        return Observable::get(subscribe);
    };

    return debounce;
}

static observable_op_t throttle_with_mapper_(const mapper_t<rx_observable_t, Variant>& throttle_duration_mapper) {

    observable_op_t throttle_with_mapper = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {

            auto cancelable = std::make_shared<SerialDisposable>();
            auto has_value = std::make_shared<bool>(false);
            auto value = std::make_shared<Variant>();
            auto _id = std::make_shared<int>(0);

            on_next_t on_next = [=](const Variant& x) {
                rx_observable_t throttle;
                try {
                    throttle = throttle_duration_mapper(x);
                } catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                *has_value = true;
                *value = x;
                ++(*_id);
                int current_id = *_id;
                auto d = std::make_shared<SingleAssignmentDisposable>();
                cancelable->set_disposable(d);

                on_next_t throttle_on_next = [=](const Variant&) {
                    if (*has_value && *_id == current_id) {
                        observer->on_next(*value);
                    }

                    *has_value = false;
                    d->dispose();
                };

                on_completed_t throttle_on_completed = [=]() {
                    if (*has_value && *_id == current_id) {
                        observer->on_next(*value);
                    }

                    *has_value = false;
                    d->dispose();
                };

                d->set_disposable(throttle->subscribe(
                    throttle_on_next, ONERROR_FWD(observer), throttle_on_completed, scheduler_
                ));
            };

            on_error_t on_error = [=](const std::exception_ptr& ex) {
                cancelable->dispose();
                observer->on_error(ex);
                *has_value = false;
                ++(*_id);
            };

            on_completed_t on_completed = [=]() {
                cancelable->dispose();
                if (*has_value) {
                    observer->on_next(*value);
                }

                observer->on_completed();
                *has_value = false;
                ++(*_id);
            };

            auto subscription = source->subscribe(
                on_next, on_error, on_completed, scheduler_
            );
            return std::make_shared<CompositeDisposable>(subscription, cancelable);
        };

        return Observable::get(subscribe);
    };

    return throttle_with_mapper;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_DEBOUNCE_H
