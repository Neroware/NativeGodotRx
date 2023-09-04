#ifndef RX_OBSERVABLE_OPERATORS_MERGE_H
#define RX_OBSERVABLE_OPERATORS_MERGE_H

#include "observable/observable.h"

#include "observable/fromiterable.h"

#include "wrapper/observable.h"

#include "disposable/compositedisposable.h"
#include "disposable/singleassignmentdisposable.h"

#include "internal/utils.h"

using namespace rx::disposable;

namespace rx::observable::op {

static observable_op_t merge_all_() {
    observable_op_t merge_all = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto group = std::make_shared<CompositeDisposable>();
            auto is_stopped = std::make_shared<bool>(false);
            auto m = std::make_shared<SingleAssignmentDisposable>();
            group->add(m);

            on_next_t on_next = [=](Ref<RxObservable> inner_source_) {
                auto inner_subscription = std::make_shared<SingleAssignmentDisposable>();
                group->add(inner_subscription);

                rx_observable_t inner_source = RxObservable::unwrap(inner_source_);

                on_completed_t on_completed_inner = [=]() {
                    std::lock_guard<RLock> guard(source->lock);
                    group->remove(inner_subscription);
                    if (*is_stopped && group->size() == 1) {
                        observer->on_completed();
                    }
                };

                on_next_t on_next_inner = [=](const Variant& x) {
                    std::lock_guard<RLock> guard(source->lock);
                    observer->on_next(x);
                };

                on_error_t on_error_inner = [=](const std::exception_ptr& exn) {
                    std::lock_guard<RLock> guard(source->lock);
                    observer->on_error(exn);
                };

                auto subscription = inner_source->subscribe(
                    on_next_inner, on_error_inner, 
                    on_completed_inner, scheduler
                );
                inner_subscription->set_disposable(subscription);
            };

            on_completed_t on_completed = [=]() {
                *is_stopped = true;
                if (group->size() == 1) {
                    observer->on_completed();
                }
            };

            m->set_disposable(source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            ));
            return group;
        };

        return Observable::get(subscribe);
    };

    return merge_all;
}

template<typename IterableT>
static rx_observable_t _merge(const IterableT& sources) {
    return from_iterable_(sources)->pipe(merge_all_());
}

template <typename IterableT>
static observable_op_t merge_with_(const IterableT& sources, int64_t max_concurrent = -1) {

    observable_op_t merge_with = OP(source) {

        if (max_concurrent < 0) {
            observable_list_t _sources(sources.begin(), sources.end());
            _sources.push_front(source);
            return _merge(_sources);
        }

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto active_count = std::make_shared<int64_t>(0);
            auto group = std::make_shared<CompositeDisposable>();
            auto is_stopped = std::make_shared<bool>(false);
            auto queue = std::make_shared<observable_list_t>();

            auto subscribe_inner = [=](const rx_observable_t& xs, const auto& _subscribe_inner) -> void {
                auto subscription = std::make_shared<SingleAssignmentDisposable>();
                group->add(subscription);

                on_completed_t on_completed_inner = [=]() {
                    std::lock_guard<RLock> guard(source->lock);
                    group->remove(subscription);
                    if (!queue->empty()) {
                        auto s = queue->front();
                        queue->pop_front();
                        _subscribe_inner(s, _subscribe_inner);
                    } else {
                        (*active_count)--;
                        if ((*is_stopped) && (*active_count) == 0) {
                            observer->on_completed();
                        }
                    }
                };

                on_next_t on_next_inner = [=](const Variant& x) {
                    std::lock_guard<RLock> guard(source->lock);
                    observer->on_next(x);
                };

                on_error_t on_error_inner = [=](const std::exception_ptr& exn) {
                    std::lock_guard<RLock> guard(source->lock);
                    observer->on_error(exn);
                };

                subscription->set_disposable(xs->subscribe(
                    on_next_inner, on_error_inner, 
                    on_completed_inner, scheduler
                ));
            };

            on_next_t on_next = [=](const rx_observable_t inner_source) {
                if (*active_count < max_concurrent) {
                    (*active_count)++;
                    subscribe_inner(inner_source, subscribe_inner);
                } else {
                    queue->push_back(inner_source);
                }
            };

            on_completed_t on_completed = [=]() {
                (*is_stopped) = true;
                if ((*active_count) == 0) {
                    observer->on_completed();
                }
            };

            group->add(source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            ));

            return group;
        };

        return Observable::get(subscribe);
    };

    return merge_with;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MERGE_H
