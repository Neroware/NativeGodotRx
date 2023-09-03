/*#ifndef RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H
#define RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H

#include "observable/observable.h"
#include "subject/subject.h"
#include "typing.h"

namespace rx::observable::op {

static observable_op_t group_by_until_(
    mapper_t<Variant, Variant> key_mapper,
    mapper_t<Variant, Variant> element_mapper,
    mapper_t<GroupedObservable<Variant, Variant>, Observable<Variant>> duration_mapper,
    rx::func::Func<Subject<Variant>()> subject_mapper
) {
    observable_op_t group_by_until = OP(source) {
        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto writers = rx::unordered_map<Variant, Subject<Variant>>();
            auto group_disposable = rx::composite_disposable();
            auto ref_count_disposable = rx::ref_count_disposable(group_disposable);

            on_next_t on_next = [=](const Variant& x) {
                auto writer = Subject<Variant>::null();
                auto key = Variant::null();

                try {
                    key = key_mapper(x);
                } catch (const std::exception& e) {
                    for (const auto& wrt : writers.values()) {
                        wrt.on_error(e);
                    }

                    observer->on_error(e);
                    return;
                }

                bool fire_new_map_entry = false;
                writer = writers[key];
                if (!writer) {
                    try {
                        writer = subject_mapper();
                    } catch (const std::exception& e) {
                        for (const auto& wrt : writers.values()) {
                            wrt.on_error(e);
                        }

                        observer->on_error(e);
                        return;
                    }

                    writers[key] = writer;
                    fire_new_map_entry = true;
                }

                if (fire_new_map_entry) {
                    auto group = GroupedObservable<Variant, Variant>(key, writer, ref_count_disposable);
                    auto duration_group = GroupedObservable<Variant, Variant>(key, writer);

                    rx::observable<Variant> duration;
                    try {
                        duration = duration_mapper(duration_group);
                    } catch (const std::exception& e) {
                        for (const auto& wrt : writers.values()) {
                            wrt.on_error(e);
                        }

                        observer->on_error(e);
                        return;
                    }

                    observer->on_next(group);
                    auto sad = rx::single_assignment_disposable();
                    group_disposable.add(sad);

                    auto expire = [&]() {
                        if (writers[key]) {
                            writers.erase(key);
                            writer.on_completed();
                        }

                        group_disposable.remove(sad);
                    };

                    auto on_next_ignore = [](const Variant&) {};
                    auto on_error_fn = [&](const std::exception& exn) {
                        for (const auto& wrt : writers.values()) {
                            wrt.on_error(exn);
                        }

                        observer->on_error(exn);
                    };

                    auto on_completed_fn = [&]() {
                        expire();
                    };

                    sad.disposable = duration.pipe(
                        rx::operators::take(1)
                    ).subscribe(on_next_ignore, on_error_fn, on_completed_fn, scheduler);

                }

                auto element = Variant::null();
                try {
                    element = element_mapper(x);
                } catch (const std::exception& error) {
                    for (const auto& wrt : writers.values()) {
                        wrt.on_error(error);
                    }

                    observer->on_error(error);
                    return;
                }

                writer.on_next(element);
            };

            auto on_error_fn = [&](const std::exception& exn) {
                for (const auto& wrt : writers.values()) {
                    wrt.on_error(exn);
                }

                observer->on_error(exn);
            };

            auto on_completed_fn = [&]() {
                for (const auto& wrt : writers.values()) {
                    wrt.on_completed();
                }

                observer->on_completed();
            };

            group_disposable.add(source.subscribe(on_next, on_error_fn, on_completed_fn, scheduler));
            return ref_count_disposable;
        };

        return Observable::get(subscribe);
    };

    return group_by_until;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_GROUPBYUNTIL_H
*/