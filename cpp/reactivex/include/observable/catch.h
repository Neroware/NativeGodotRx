#ifndef RX_OBSERVABLE_CATCH_H
#define RX_OBSERVABLE_CATCH_H

#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

template<typename T>
static std::shared_ptr<Observable> catch_with_iterable_(const T& sources) {
    auto _end = sources.end();
    auto _it = std::make_shared<typename T::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        auto last_exception = std::make_shared<std::exception_ptr>();
        auto is_disposed = std::make_shared<bool>(false);

        auto action = [=](const std::shared_ptr<SchedulerBase>& scheduler__, const Variant& state, const auto& _action) -> std::shared_ptr<DisposableBase> {
            on_error_t on_error = [=](const std::exception_ptr& exn) {
                *last_exception = exn;
                cancelable->set_disposable(_scheduler->schedule(ACTION { return _action(scheduler__, state, _action); }));
            };

            if (*is_disposed) {
                return nullptr;
            }

            std::shared_ptr<ObservableBase> current;
            try { 
                if (*_it != _end) {
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
                [observer](const Variant& value) { observer->on_next(value); },
                on_error,
                [observer]() { observer->on_completed(); },
                scheduler_
            ));

            return nullptr;
        };

        cancelable->set_disposable(_scheduler->schedule(ACTION { return action(scheduler__, state, action); } ));

        dispose_t dispose = [is_disposed]() {
            *is_disposed = true;
        };

        return std::make_shared<CompositeDisposable>(std::list<std::shared_ptr<DisposableBase>>{
            subscription, cancelable, std::make_shared<Disposable>(dispose)
        });
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_CATCH_H