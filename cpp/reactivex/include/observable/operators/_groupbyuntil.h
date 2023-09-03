#ifndef RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H
#define RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H

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

typedef std::shared_ptr<GroupedObservable> grouped_observable_t;
typedef std::shared_ptr<Subject> rx_subject_t;

static observable_op_t group_by_until_(
    const mapper_t<Variant, Variant>& key_mapper,
    const mapper_t<rx_observable_t, grouped_observable_t>& duration_mapper,
    const mapper_t<Variant, Variant>& element_mapper = nullptr,
    const mapper_t<rx_subject_t>& subject_mapper = nullptr
) {

    auto element_mapper_ = element_mapper ? element_mapper : basic::identity<const Variant&>;
    auto subject_mapper_ = subject_mapper ? subject_mapper : []() -> rx_subject_t { return Subject::get(); };

    observable_op_t group_by_until = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto writers = std::make_shared<std::map<Variant, rx_subject_t, variant_compare>>();
            auto group_disposable = std::make_shared<CompositeDisposable>();
            auto ref_count_disposable = RefCountDisposable::get(group_disposable);

            on_next_t on_next = [=](const Variant& x) {
                rx_subject_t writer = nullptr;
                Variant key = VNULL;

                try {
                    key = key_mapper(x);
                } catch (...) {
                    for (const auto& wrt : *writers) {
                        wrt.second->on_error(std::current_exception());
                    }

                    observer->on_error(std::current_exception());
                    return;
                }

                bool fire_new_map_entry = false;
                bool writer_contained = writers->find(key) != writers->end();
                if (!writer_contained) {
                    try {
                        writer = subject_mapper_();
                    } catch (...) {
                        for (const auto& wrt : *writers) {
                            wrt.second->on_error(std::current_exception());
                        }

                        observer->on_error(std::current_exception());
                        return;
                    }

                    (*writers)[key] = writer;
                    fire_new_map_entry = true;
                }
                writer = (*writers)[key];

                if (fire_new_map_entry) {
                    auto group = GroupedObservable::get(key, writer, ref_count_disposable);
                    auto duration_group = GroupedObservable::get(key, writer);

                    rx_observable_t duration;
                    try {
                        duration = duration_mapper(duration_group);
                    } catch (...) {
                        for (const auto& wrt : *writers) {
                            wrt.second->on_error(std::current_exception());
                        }

                        observer->on_error(std::current_exception());
                        return;
                    }

                    observer->on_next(RxGroupedObservable::wrap(group));
                    auto sad = std::make_shared<SingleAssignmentDisposable>();
                    group_disposable->add(sad);

                    auto expire = [=]() {
                        if ((*writers)[key]) {
                            writers->erase(key);
                            writer->on_completed();
                        }

                        group_disposable->remove(sad);
                    };

                    on_next_t on_next = [](const Variant&) {};

                    on_error_t on_error = [=](const std::exception_ptr& exn) {
                        for (const auto& wrt : (*writers)) {
                            wrt.second->on_error(exn);
                        }

                        observer->on_error(exn);
                    };

                    auto on_completed = [expire]() {
                        expire();
                    };

                    sad->set_disposable(duration->pipe(take_(1))->subscribe(
                        on_next, on_error, 
                        on_completed, scheduler
                    ));
                }

                auto element = VNULL;
                try {
                    element = element_mapper_(x);
                } catch (...) {
                    for (const auto& wrt : (*writers)) {
                        wrt.second->on_error(std::current_exception());
                    }

                    observer->on_error(std::current_exception());
                    return;
                }

                writer->on_next(element);
            };

            on_error_t on_error = [=](const std::exception_ptr& exn) {
                for (const auto& wrt : (*writers)) {
                    wrt.second->on_error(exn);
                }

                observer->on_error(exn);
            };

            on_completed_t on_completed = [=]() {
                for (const auto& wrt : (*writers)) {
                    wrt.second->on_completed();
                }

                observer->on_completed();
            };

            group_disposable->add(source->subscribe(
                on_next, on_error, 
                on_completed, scheduler
            ));
            return ref_count_disposable;
        };

        return Observable::get(subscribe);
    };

    return group_by_until;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H