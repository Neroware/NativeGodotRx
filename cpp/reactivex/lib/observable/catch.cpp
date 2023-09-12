#include "observable/definitions.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::catch_with_iterable_(const iterable_t& sources) {
    auto _it = sources->iter();

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        auto last_exception = std::make_shared<error_t>();
        auto is_disposed = std::make_shared<bool>(false);

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            on_error_t on_error = [=](const error_t& exn) {
                *last_exception = exn;
                cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(_action)));
            };

            if (*is_disposed) {
                return nullptr;
            }

            rx_observable_t current;
            try { 
                if (_it->has_next()) {
                    current = _it->next();
                }
                else {
                    if (*last_exception) {
                        observer->on_error(*last_exception);
                    }
                    else {
                        observer->on_completed();
                    }
                    return nullptr;
                }
            }
            catch (...) {
                observer->on_error(std::current_exception());
                return nullptr;
            }
            auto d = std::make_shared<SingleAssignmentDisposable>();
            subscription->set_disposable(d);
            d->set_disposable(current->subscribe(
                ONNEXT_FWD(observer), on_error,
                ONCOMPLETED_FWD(observer), scheduler_
            ));

            return nullptr;
        };

        cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(action)));

        dispose_t dispose = [is_disposed]() {
            *is_disposed = true;
        };

        return std::make_shared<CompositeDisposable>(
            subscription, cancelable, std::make_shared<Disposable>(dispose)
        );
    };

    return Observable::get(subscribe);
}

rx_observable_t obs::catch_(const rx_observable_list_t& sources) {
    auto _it = std::make_shared<rx_observable_list_t::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        auto last_exception = std::make_shared<error_t>();
        auto is_disposed = std::make_shared<bool>(false);

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            on_error_t on_error = [=](const error_t& exn) {
                *last_exception = exn;
                cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(_action)));
            };

            if (*is_disposed) {
                return nullptr;
            }

            rx_observable_t current;
            try { 
                if (*_it != sources.end()) {
                    current = **_it; (*_it)++;
                }
                else {
                    if (*last_exception) {
                        observer->on_error(*last_exception);
                    }
                    else {
                        observer->on_completed();
                    }
                    return nullptr;
                }
            }
            catch (...) {
                observer->on_error(std::current_exception());
                return nullptr;
            }
            auto d = std::make_shared<SingleAssignmentDisposable>();
            subscription->set_disposable(d);
            d->set_disposable(current->subscribe(
                ONNEXT_FWD(observer), on_error,
                ONCOMPLETED_FWD(observer), scheduler_
            ));

            return nullptr;
        };

        cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(action)));

        dispose_t dispose = [is_disposed]() {
            *is_disposed = true;
        };

        return std::make_shared<CompositeDisposable>(
            subscription, cancelable, std::make_shared<Disposable>(dispose)
        );
    };

    return Observable::get(subscribe);
}
rx_observable_t obs::catch_(const rx_observable_t& sources...) {
    return catch_({sources});
}

} // END namespace rx::observable