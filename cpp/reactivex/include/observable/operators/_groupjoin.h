#ifndef RX_OBSERVABLE_OPERATORS_GROUPJOIN_H
#define RX_OBSERVABLE_OPERATORS_GROUPJOIN_H

#include "observable/observable.h"
#include "observable/groupedobservable.h"

#include "observable/operators/_take.h"

#include "internal/utils.h"

#include "wrapper/observable.h"

#include "subject/subject.h"
#include "typing.h"

#include <map>

using namespace rx::wrappers;
using namespace rx::subject;

namespace rx::observable::op {

typedef std::shared_ptr<Subject> rx_subject_t;

static observable_op_t group_join_(
    const rx_observable_t& right,
    const mapper_t<rx_observable_t, Variant>& left_duration_mapper,
    const mapper_t<rx_observable_t, Variant>& right_duration_mapper
) {

    on_next_t nothing = [](const Variant& __) {};

    observable_op_t group_join = OP(left) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto group = std::make_shared<CompositeDisposable>();
            auto rcd = RefCountDisposable::get(group);
            auto left_map = std::make_shared<std::map<uint64_t, rx_subject_t>>();
            auto right_map = std::make_shared<std::map<uint64_t, Variant>>();
            auto left_id = std::make_shared<uint64_t>(0);
            auto right_id = std::make_shared<uint64_t>(0);

            on_next_t on_next_left = [=](const Variant& value) {
                rx_subject_t subject = Subject::get();

                uint64_t _id;
                {
                    std::lock_guard<RLock> guard(left->lock);
                    _id = *left_id;
                    ++(*left_id);
                    (*left_map)[_id] = subject;
                }

                Variant result;
                try {
                    result = Array::make(value, RxObservable::wrap(add_ref(subject, rcd)));
                } catch (...) {
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_error(std::current_exception());
                    }

                    observer->on_error(std::current_exception());
                    return;
                }

                observer->on_next(result);

                for (const auto& right_value : *right_map) {
                    subject->on_next(right_value.second);
                }

                auto md = std::make_shared<SingleAssignmentDisposable>();
                group->add(md);

                auto expire = [=]() {
                    if (left_map->find(_id) != left_map->end()) {
                        left_map->erase(_id);
                        subject->on_completed();
                    }

                    group->remove(md);
                };

                rx_observable_t duration;
                try {
                    duration = left_duration_mapper(value);
                } catch (...) {
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_error(std::current_exception());
                    }

                    observer->on_error(std::current_exception());
                    return;
                }

                on_error_t on_error = [=](const std::exception_ptr& exn) {
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_error(exn);
                    }

                    observer->on_error(exn);
                };

                md->set_disposable(duration->pipe(take_(1))->subscribe(
                    nothing, on_error, expire, scheduler
                ));
            };

            on_error_t on_error_left = [=](const std::exception_ptr& exn) {
                for (const auto& left_value : *left_map) {
                    left_value.second->on_error(exn);
                }

                observer->on_error(exn);
            };

            group->add(right->subscribe(
                on_next_left,
                on_error_left,
                ONCOMPLETED_FWD(observer),
                scheduler
            ));

            on_next_t send_right = [=](const Variant& value) {
                uint64_t _id;
                {
                    std::lock_guard<RLock> guard(left->lock);
                    _id = *right_id;
                    ++(*right_id);
                    (*right_map)[_id] = value;
                }

                auto md = std::make_shared<SingleAssignmentDisposable>();
                group->add(md);

                auto expire = [=]() {
                    (*right_map).erase(_id);
                    group->remove(md);
                };

                rx_observable_t duration;
                try {
                    duration = right_duration_mapper(value);
                } catch (...) {
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_error(std::current_exception());
                    }

                    observer->on_error(std::current_exception());
                    return;
                }

                on_error_t on_error = [=](const std::exception_ptr& exn) {
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_error(exn);
                    }

                    observer->on_error(exn);
                };

                md->set_disposable(duration->pipe(take_(1))->subscribe(
                    nothing, on_error, expire, scheduler
                ));

                {
                    std::lock_guard<RLock>(left->lock);
                    for (const auto& left_value : *left_map) {
                        left_value.second->on_next(value);
                    }
                }
            };

            auto on_error_right = [=](const std::exception_ptr& exn) {
                for (const auto& left_value : *left_map) {
                    left_value.second->on_error(exn);
                }

                observer->on_error(exn);
            };

            group->add(right->subscribe(
                send_right, on_error_right,
                DEFAULT_ON_COMPLETED, scheduler
            ));

            return rcd;
        };

        return Observable::get(subscribe);
    };

    return group_join;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_GROUPJOIN_H
