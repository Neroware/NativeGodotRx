#ifndef RX_OBSERVABLE_OPERATORS_AMB_H
#define RX_OBSERVABLE_OPERATORS_AMB_H

#include "observable/observable.h"

#include "typing.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

observable_op_t amb_(const std::shared_ptr<Observable>& right_source) {

    auto obs = right_source;

    observable_op_t amb = OP(left_source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto choice = std::make_shared<uint8_t>(0);
            uint8_t left_choice = 1;
            uint8_t right_choice = 2;
            auto left_subscription = std::make_shared<SingleAssignmentDisposable>();
            auto right_subscription = std::make_shared<SingleAssignmentDisposable>();

            auto choice_left = [=]() {
                if (!*choice) {
                    *choice = left_choice;
                    right_subscription->dispose();
                }
            };

            auto choice_right = [=]() {
                if (!*choice) {
                    *choice = right_choice;
                    left_subscription->dispose();
                }
            };

            on_next_t on_next_left = [=](const Variant& value) {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_left();
                }
                if (*choice == left_choice) {
                    observer->on_next(value);
                }
            };

            on_error_t on_error_left = [=](const std::exception_ptr& err) {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_left();
                }
                if (*choice == left_choice) {
                    observer->on_error(err);
                }
            };

            on_completed_t on_completed_left = [=]() {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_left();
                }
                if (*choice == left_choice) {
                    observer->on_completed();
                }
            };

            auto left_d = left_source->subscribe(
                on_next_left, on_error_left, on_completed_left, scheduler);
            left_subscription->set_disposable(left_d);

            on_next_t on_next_right = [=](const Variant& value) {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_right();
                }
                if (*choice == right_choice) {
                    observer->on_next(value);
                }
            };

            on_error_t on_error_right = [=](const std::exception_ptr& err) {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_right();
                }
                if (*choice == right_choice) {
                    observer->on_error(err);
                }
            };

            on_completed_t on_completed_right = [=]() {
                {
                    std::lock_guard<RLock> guard(left_source->lock);
                    choice_right();
                }
                if (*choice == right_choice) {
                    observer->on_completed();
                }
            };

            auto right_d = obs->subscribe(
                on_next_right, on_error_right, on_completed_right, scheduler);
            right_subscription->set_disposable(right_d);

            return std::make_shared<CompositeDisposable>(disposable_list_t{left_subscription, right_subscription});
        };

        return Observable::get(subscribe);
    };

    return amb;
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_OPERATORS_AMB_H