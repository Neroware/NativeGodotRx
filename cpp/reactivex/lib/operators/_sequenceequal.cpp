#include "observable/definitions.h"

#include "disposable/compositedisposable.h"

namespace rx::observable::op {

observable_op_t ops::sequence_equal_(const iterable_t& second, const comparer_t<Variant>& comparer) {
    return ops::sequence_equal_(from_iterable(second), comparer);
} 

observable_op_t ops::sequence_equal_(
    const rx_observable_t& second,
    const comparer_t<Variant>& comparer
) {

    comparer_t<Variant> comparer_ = comparer ? comparer : basic::default_comparer<const Variant&>;
    rx_observable_t second_ = second;

    auto sequence_equal = OP(source) {
        auto first = source;

        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto donel = std::make_shared<bool>(false);
            auto doner = std::make_shared<bool>(false);
            auto ql = std::make_shared<std::list<Variant>>();
            auto qr = std::make_shared<std::list<Variant>>();

            on_next_t on_next1 = [=](const Variant& x) {
                if (!qr->empty()) {
                    Variant v = qr->front();
                    qr->pop_front();
                    bool equal;
                    try {
                        equal = comparer_(v, x);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                    }
                    if (!equal) {
                        observer->on_next(false);
                        observer->on_completed();
                    }
                } else if (*doner) {
                    observer->on_next(false);
                    observer->on_completed();
                } else {
                    ql->push_back(x);
                }
            };

            on_completed_t on_completed1 = [=]() {
                *donel = true;
                if (ql->empty()) {
                    if (!qr->empty()) {
                        observer->on_next(false);
                        observer->on_completed();
                    } else if (*doner) {
                        observer->on_next(true);
                        observer->on_completed();
                    }
                }
            };

            on_next_t on_next2 = [=](const Variant& x) {
                if (!ql->empty()) {
                    Variant v = ql->front();
                    ql->pop_front();
                    bool equal;
                    try {
                        equal = comparer_(v, x);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                    }
                    if (!equal) {
                        observer->on_next(false);
                        observer->on_completed();
                    }
                } else if (*donel) {
                    observer->on_next(false);
                    observer->on_completed();
                } else {
                    qr->push_back(x);
                }
            };

            on_completed_t on_completed2 = [=]() {
                *doner = true;
                if (qr->empty()) {
                    if (!ql->empty()) {
                        observer->on_next(false);
                        observer->on_completed();
                    } else if (*donel) {
                        observer->on_next(true);
                        observer->on_completed();
                    }
                }
            };

            auto subscription1 = first->subscribe(on_next1, ONERROR_FWD(observer), on_completed1, scheduler);
            auto subscription2 = second_->subscribe(on_next2, ONERROR_FWD(observer), on_completed2, scheduler);

            return std::make_shared<CompositeDisposable>(subscription1, subscription2);
        };

        return Observable::get(subscribe);
    };

    return sequence_equal;
}

} // END namespace rx::observable::op
