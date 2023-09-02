#ifndef RX_OBSERVABLE_OPERATORS_EXPAND_H
#define RX_OBSERVABLE_OPERATORS_EXPAND_H

#include "observable/observable.h"
#include "disposable/serialdisposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"
#include "scheduler/immediatescheduler.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

static observable_op_t expand_(const mapper_t<rx_observable_t, Variant>& mapper) {

    observable_op_t expand = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            scheduler_t scheduler = scheduler_ ? scheduler_ : ImmediateScheduler::singleton();

            auto queue = std::make_shared<observable_list_t>();
            auto m = std::make_shared<SingleAssignmentDisposable>();
            auto d = std::make_shared<CompositeDisposable>();
            auto active_count = std::make_shared<uint64_t>(0);
            auto is_acquired = std::make_shared<bool>(false);

            auto ensure_active = [=](const auto& _ensure_active) -> void {

                bool is_owner = false;
                if (!queue->empty()) {
                    is_owner = !*is_acquired;
                    *is_acquired = true;
                }

                auto action = RECURSIVE_ACTION(scheduler, state, _action) {
                    rx_observable_t work;
                    if (queue->empty()) {
                        work = queue->front();
                        queue->pop_front();
                    }
                    else {
                        *is_acquired = false;
                        return nullptr;
                    }

                    auto sad = std::make_shared<SingleAssignmentDisposable>();
                    d->add(sad);

                    on_next_t on_next = [=](const Variant& value) {
                        observer->on_next(value);

                        rx_observable_t result;
                        try {
                            result = mapper(value);
                        } catch (...) {
                            observer->on_error(std::current_exception());
                            return;
                        }

                        queue->push_back(result);
                        (*active_count)++;
                        _ensure_active(_ensure_active);
                    };

                    on_completed_t on_completed = [=]() {
                        d->remove(sad);
                        (*active_count)--;

                        if (*active_count == 0) {
                            observer->on_completed();
                        }
                    };

                    sad->set_disposable(work->subscribe(
                        on_next, ONERROR_FWD(observer), on_completed, scheduler
                    ));
                    m->set_disposable(scheduler->schedule(RECURSIVE_ACTION_FWD(_action)));

                    return nullptr;
                };

                if (is_owner) {
                    m->set_disposable(scheduler->schedule(RECURSIVE_ACTION_FWD(action)));
                }
            };

            queue->push_back(source);
            (*active_count)++;
            ensure_active(ensure_active);

            return d;
        };

        return Observable::get(subscribe);
    };

    return expand;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_EXPAND_H
