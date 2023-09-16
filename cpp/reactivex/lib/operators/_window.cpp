#include "observable/definitions.h"

#include "subject/subject.h"

#include "disposable/compositedisposable.h"
#include "disposable/refcountdisposable.h"
#include "disposable/serialdisposable.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::window_toggle_(
    const rx_observable_t& openings, 
    const mapper_t<rx_observable_t, Variant>& closing_mapper
) {

    observable_op_t window_toggle = OP(source) {

        auto mapper = [](const Array& args) -> rx_observable_t {
            rx_observable_t window = args[1];
            return window;
        };

        return openings->pipe(
            group_join(
                source,
                closing_mapper,
                [](const Variant&) -> rx_observable_t { return empty(); }
            ),
            map(mapper)
        );
    };

    return window_toggle;
}

observable_op_t ops::window_(const rx_observable_t& boundaries) {

    observable_op_t window = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto window_subject =  std::make_shared<rx_subject_t>(Subject::get());
            auto d = std::make_shared<CompositeDisposable>();
            auto r = RefCountDisposable::get(d);

            observer->on_next(add_ref(*window_subject, r));

            auto on_next_window = [=](const Variant& x) {
                (*window_subject)->on_next(x);
            };

            auto on_error = [=](const std::exception_ptr& err) {
                (*window_subject)->on_error(err);
                observer->on_error(err);
            };

            auto on_completed = [=] {
                (*window_subject)->on_completed();
                observer->on_completed();
            };

            d->add(
                source->subscribe(
                    on_next_window, on_error, on_completed, scheduler_
                )
            );

            auto on_next_observer = [=](const rx_observable_t& w) {
                (*window_subject)->on_completed();
                (*window_subject) = Subject::get();
                observer->on_next(add_ref(*window_subject, r));
            };

            d->add(
                boundaries->subscribe(
                    on_next_observer, on_error, on_completed, scheduler_
                )
            );

            return r;
        };

        return Observable::get(subscribe);
    };

    return window;
}

observable_op_t ops::window_when_(const mapper_t<rx_observable_t>& closing_mapper) {

    observable_op_t window_when = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto m = std::make_shared<SerialDisposable>();
            auto d = std::make_shared<CompositeDisposable>();
            auto r = RefCountDisposable::get(d);
            auto window = std::make_shared<rx_subject_t>(Subject::get());

            observer->on_next(add_ref(*window, r));

            auto on_next = [=](const Variant& value) {
                (*window)->on_next(value);
            };

            auto on_error = [=](const std::exception_ptr& error) {
                (*window)->on_error(error);
                observer->on_error(error);
            };

            auto on_completed = [=] {
                (*window)->on_completed();
                observer->on_completed();
            };

            d->add(
                source->subscribe(on_next, on_error, on_completed, scheduler_)
            );

            auto create_window_on_completed = [=](const auto& _create_window_on_completed) {
                rx_observable_t window_close;
                try {
                    window_close = closing_mapper();
                } catch (const std::exception_ptr& exception) {
                    observer->on_error(exception);
                    return;
                }

                auto on_window_completed = [=]() {
                    (*window)->on_completed();
                    (*window) = Subject::get();
                    observer->on_next(add_ref(*window, r));
                    _create_window_on_completed(_create_window_on_completed);
                };

                auto m1 = std::make_shared<SingleAssignmentDisposable>();
                m->set_disposable(m1);
                m1->set_disposable(window_close->pipe(take(1))->subscribe(
                    DEFAULT_ON_NEXT, on_error, 
                    on_window_completed, scheduler_
                ));
            };

            create_window_on_completed(create_window_on_completed);
            return r;
        };

        return Observable::get(subscribe);
    };

    return window_when;
}

} // END namespace rx::observable::op
