#ifndef RX_OBSERVABLE_OPERATORS_DO_ACTION_H
#define RX_OBSERVABLE_OPERATORS_DO_ACTION_H

#include "observable/observable.h"

#include "disposable/compositedisposable.h"

#include "internal/utils.h"
#include "typing.h"

namespace rx::observable::op {

typedef std::function<void()> do_action_t;
static do_action_t do_action_cb(const Callable& cb) {
    return [cb]() { cb.callv(Array()); };
}

static observable_op_t do_action_(
    const on_next_t& on_next = nullptr,
    const on_error_t& on_error = nullptr,
    const on_completed_t& on_completed = nullptr
) {

    observable_op_t do_action = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            on_next_t _on_next = [=](const Variant& x) {
                if (!on_next) {
                    observer->on_next(x);
                } else {
                    try {
                        on_next(x);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                    }
                    observer->on_next(x);
                }
            };

            on_error_t _on_error = [=](const std::exception_ptr& exception) {
                if (!on_error) {
                    observer->on_error(exception);
                } else {
                    try {
                        on_error(exception);
                    } catch (...) {
                        observer->on_error(std::current_exception());
                    }
                    observer->on_error(exception);
                }
            };

            on_completed_t _on_completed = [=]() {
                if (!on_completed) {
                    observer->on_completed();
                } else {
                    try {
                        on_completed();
                    } catch (...) {
                        observer->on_error(std::current_exception());
                    }
                    observer->on_completed();
                }
            };

            return source->subscribe(
                _on_next, _on_error, _on_completed, scheduler_
            );
        };

        return Observable::get(subscribe);
    };

    return do_action;
}

static observable_op_t do_(const observer_t& observer) {
    return do_action_(ONNEXT_FWD(observer), ONERROR_FWD(observer), ONCOMPLETED_FWD(observer));
}

static rx_observable_t do_after_next(const rx_observable_t& source, const on_next_t& after_next) {
    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        auto on_next = [=](const Variant& value) {
            try {
                observer->on_next(value);
                after_next(value);
            } catch (...) {
                observer->on_error(std::current_exception());
            }
        };

        return source->subscribe(
            on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer)
        );
    };

    return Observable::get(subscribe);
}



static rx_observable_t do_on_subscribe(const rx_observable_t& source, const do_action_t& on_subscribe) {
    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        on_subscribe();
        return source->subscribe(
            ONNEXT_FWD(observer), ONERROR_FWD(observer), 
            ONCOMPLETED_FWD(observer), scheduler
        );
    };

    return Observable::get(subscribe);
}


static rx_observable_t do_on_dispose(const rx_observable_t& source, const dispose_t& on_dispose) {
    class OnDispose : public DisposableBase {
    public:
        OnDispose(const dispose_t& on_dispose_) 
            : on_dispose(on_dispose_) {}
        void dispose() override {
            on_dispose();
        }
    private:
        dispose_t on_dispose;
    };

    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        auto composite_disposable = std::make_shared<CompositeDisposable>();
        composite_disposable->add(std::make_shared<OnDispose>(on_dispose));

        auto subscription = source->subscribe(
            ONNEXT_FWD(observer), ONERROR_FWD(observer),
            ONCOMPLETED_FWD(observer), scheduler
        );
        composite_disposable->add(subscription);

        return composite_disposable;
    };

    return Observable::get(subscribe);
}


static rx_observable_t do_on_terminate(const rx_observable_t& source, const do_action_t& on_terminate) {
    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        on_completed_t on_completed = [=]() {
            try {
                on_terminate();
            } catch (...) {
                observer->on_error(std::current_exception());
                return;
            }
            observer->on_completed();
        };

        on_error_t on_error = [=](const std::exception_ptr& exception) {
            try {
                on_terminate();
            } catch (...) {
                observer->on_error(std::current_exception());
                return;
            }
            observer->on_error(exception);
        };

        return source->subscribe(ONNEXT_FWD(observer), on_error, on_completed, scheduler);
    };

    return Observable::get(subscribe);
}

static rx_observable_t do_after_terminate(const rx_observable_t& source, const do_action_t& after_terminate) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        on_completed_t on_completed = [=]() {
            observer->on_completed();
            try {
                after_terminate();
            } catch (...) {
                observer->on_error(std::current_exception());
            }
        };

        on_error_t on_error = [=](const std::exception_ptr& exception) {
            observer->on_error(exception);
            try {
                after_terminate();
            } catch (...) {
                observer->on_error(std::current_exception());
            }
        };

        return source->subscribe(ONNEXT_FWD(observer), on_error, on_completed, scheduler);
    };

    return Observable::get(subscribe);
}


static observable_op_t do_finally_(const rx_observable_t& source, const do_action_t& finally_action) {

    class OnDispose : public DisposableBase {
    public:
        OnDispose(const do_action_t& finally_action, const std::shared_ptr<bool>& was_invoked) 
            : finally_action(finally_action), was_invoked(was_invoked) {}
        void dispose() override {
            if (!was_invoked) {
                finally_action();
                *was_invoked = true;
            }
        }
    private:
        do_action_t finally_action;
        std::shared_ptr<bool> was_invoked;
    };

    observable_op_t partial = [=](const rx_observable_t& source) -> rx_observable_t {
        auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto was_invoked = std::make_shared<bool>(false);

            on_completed_t on_completed = [=]() {
                observer->on_completed();
                try {
                    if (!*was_invoked) {
                        finally_action();
                        *was_invoked = true;
                    }
                } catch (...) {
                    observer->on_error(std::current_exception());
                }
            };

            on_error_t on_error = [=](const std::exception_ptr& exception) {
                observer->on_error(exception);
                try {
                    if (!*was_invoked) {
                        finally_action();
                        *was_invoked = true;
                    }
                } catch (...) {
                    observer->on_error(std::current_exception());
                }
            };

            auto composite_disposable = std::make_shared<CompositeDisposable>();
            composite_disposable->add(std::make_shared<OnDispose>(finally_action, was_invoked));

            auto subscription = source->subscribe(
                ONNEXT_FWD(observer), on_error, on_completed, scheduler
            );
            composite_disposable->add(subscription);

            return composite_disposable;
        };

        return Observable::get(subscribe);
    };

    return partial;
}


} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_DO_ACTION_H
