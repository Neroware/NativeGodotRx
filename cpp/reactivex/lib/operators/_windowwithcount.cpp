#include "observable/definitions.h"

#include "subject/subject.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/refcountdisposable.h"

using namespace rx::subject;

namespace rx::observable::op {

observable_op_t ops::window_with_count_(int64_t count, int64_t skip) {

    if (count <= 0) {
        throw ArgumentOutOfRangeException();
    }

    int64_t skip_ = skip >= 0 ? skip : count;

    observable_op_t window_with_count = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto m = std::make_shared<SingleAssignmentDisposable>();
            auto refCountDisposable = RefCountDisposable::get(m);
            auto n = std::make_shared<int64_t>(0);
            auto q = std::make_shared<std::list<rx_subject_t>>();

            auto create_window = [=]() {
                auto s = Subject::get();
                q->push_back(s);
                observer->on_next(add_ref(s, refCountDisposable));
            };

            create_window();

            auto on_next = [=](const Variant& x) {
                for (const auto& item : *q) {
                    item->on_next(x);
                }

                int64_t c = *n - count + 1;
                if (c >= 0 && c % skip_ == 0) {
                    auto s = q->front();
                    q->pop_front();
                    s->on_completed();
                }

                ++(*n);
                if ((*n % skip_) == 0) {
                    create_window();
                }
            };

            auto on_error = [=](const std::exception_ptr& exception) {
                while (!q->empty()) {
                    q->front()->on_error(exception);
                    q->pop_front();
                }
                observer->on_error(exception);
            };

            auto on_completed = [=]() {
                while (!q->empty()) {
                    q->front()->on_completed();
                    q->pop_front();
                }
                observer->on_completed();
            };

            m->set_disposable(source->subscribe(
                on_next, on_error, on_completed, scheduler_
            ));
            return refCountDisposable;
        };

        return Observable::get(subscribe);
    };

    return window_with_count;
}

} // END namespace rx::observable::op
