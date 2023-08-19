#ifndef RX_OBSERVABLE_CONCAT_H
#define RX_OBSERVABLE_CONCAT_H

#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

template<typename T>
static std::shared_ptr<Observable> concat_with_iterable_(const T& sources) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto _end = sources.end();
        auto _it = std::make_shared<typename T::const_iterator>(sources.begin());

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        auto is_disposed = std::make_shared<bool>(false);

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            if (*is_disposed) {
                return nullptr;
            }

            on_completed_t on_completed = [=]() {
                cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(_action)));
            };

            std::shared_ptr<Observable> current;
            try { 
                if (*_it != _end) {
                    current = **_it; (*_it)++;
                }
                else {
                    observer->on_completed();
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
                [observer](const std::exception_ptr& error) { observer->on_error(error); },
                on_completed, scheduler_
            ));

            return nullptr;
        };

        cancelable->set_disposable(_scheduler->schedule(RECURSIVE_ACTION_FWD(action)));

        dispose_t dispose = [=]() {
            *is_disposed = true;
        };

        return std::make_shared<CompositeDisposable>(std::initializer_list<std::shared_ptr<DisposableBase>>{
            subscription, cancelable, std::make_shared<Disposable>(dispose)
        });
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_CONCAT_H