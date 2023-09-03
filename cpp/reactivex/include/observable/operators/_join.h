#ifndef RX_OBSERVABLE_OPERATORS_JOIN_H
#define RX_OBSERVABLE_OPERATORS_JOIN_H

#include "observable/observable.h"
#include "observable/operators/_take.h"
#include "internal/utils.h"

#include "disposable/compositedisposable.h"
#include "disposable/singleassignmentdisposable.h"

#include <map>
#include <tuple>

using namespace rx::disposable;

namespace rx::observable::op {

static observable_op_t join_(
    const rx_observable_t& right,
    const mapper_t<rx_observable_t, Variant>& left_duration_mapper,
    const mapper_t<rx_observable_t, Variant>& right_duration_mapper
) {

    observable_op_t join = OP(left) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto group = std::make_shared<CompositeDisposable>();
            auto left_done = std::make_shared<bool>(false);
            auto left_map = std::make_shared<std::map<uint64_t, Variant>>();
            auto left_id = std::make_shared<uint64_t>(0);
            auto right_done = std::make_shared<bool>(false);
            auto right_map = std::make_shared<std::map<uint64_t, Variant>>();
            auto right_id = std::make_shared<uint64_t>(0);

            on_next_t on_next_left = [=](const Variant& value) {
                rx_observable_t duration;
                int current_id = (*left_id)++;
                auto md = std::make_shared<SingleAssignmentDisposable>();

                (*left_map)[current_id] = value;
                group->add(md);

                auto expire = [=]() {
                    if (left_map->find(current_id) != left_map->end()) {
                        left_map->erase(current_id);
                    }
                    if (left_map->empty() && *left_done) {
                        observer->on_completed();
                    }

                    group->remove(md);
                };

                try {
                    duration = left_duration_mapper(value);
                } catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                md->set_disposable(duration->pipe(take_(1))->subscribe(
                    basic::noop<const Variant&>, ONERROR_FWD(observer),
                    expire, scheduler
                ));

                for (const auto& val : *right_map) {
                    observer->on_next(Array::make(value, val.second));
                }
            };

            on_completed_t on_completed_left = [=]() {
                *left_done = true;
                if (*right_done || left_map->empty()) {
                    observer->on_completed();
                }
            };

            group->add(left->subscribe(
                on_next_left,
                ONERROR_FWD(observer),
                on_completed_left,
                scheduler
            ));

            on_next_t on_next_right = [=](const Variant& value) {
                rx_observable_t duration;
                uint64_t current_id = (*right_id)++;
                auto md = std::make_shared<SingleAssignmentDisposable>();

                (*right_map)[current_id] = value;
                group->add(md);

                auto expire = [=]() {
                    if (right_map->find(current_id) != right_map->end()) {
                        right_map->erase(current_id);
                    }
                    if (right_map->empty() && *right_done) {
                        observer->on_completed();
                    }

                    group->remove(md);
                };

                try {
                    duration = right_duration_mapper(value);
                } catch (...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                md->set_disposable(duration->pipe(take_(1))->subscribe(
                    basic::noop<const Variant&>, ONERROR_FWD(observer), expire, scheduler
                ));

                for (const auto& val : *left_map) {
                    observer->on_next(Array::make(val.second, value));
                }
            };

            on_completed_t on_completed_right = [=]() {
                *right_done = true;
                if (*left_done || right_map->empty()) {
                    observer->on_completed();
                }
            };

            group->add(right->subscribe(
                on_next_right,
                ONERROR_FWD(observer),
                on_completed_right,
                scheduler
            ));

            return group;
        };

        return Observable::get(subscribe);
    };

    return join;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_JOIN_H
