#ifndef RX_OBSERVABLE_ONERRORRESUMENEXT_H
#define RX_OBSERVABLE_ONERRORRESUMENEXT_H

#include "observable/observable.h"

#include "disposable/serialdisposable.h"

using namespace rx::scheduler;

namespace rx::observable {

static std::shared_ptr<Observable> _get_obs(const std::shared_ptr<Observable>& source, const Variant& __ = Variant()) { 
    return source; 
}
static std::shared_ptr<Observable> _get_obs(const std::function<std::shared_ptr<Observable>(const std::exception_ptr&)>& source, Ref<RxError> state = Ref<RxError>()) { 
    return source(RxError::unwrap(state)); 
}

template<typename T>
static std::shared_ptr<Observable> on_error_resume_next_(const T& sources) {

    auto _end = sources.end();
    auto _it = std::make_shared<typename T::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(nullptr) {
        
        auto scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();

        auto action = RECURSIVE_ACTION {
            std::shared_ptr<Observable> source;
            if (*_it != _end) {
                // Allow source to be a factory method taking an error
                source = _get_obs(**_it, state); (*_it)++;
            }
            else {
                observer->on_completed();
                return nullptr;
            }

            auto current = source;

            auto d = std::make_shared<SingleAssignmentDisposable>();
            subscription->set_disposable(d);

            auto on_error_resume = [=](const std::exception_ptr& state) {
                scheduler__->schedule(ACTION { return _action(scheduler__, state, _action); }, RxError::wrap(state));
            };
            auto on_resume = [=]() {
                scheduler__->schedule(ACTION { return _action(scheduler__, state, _action); });
            };
            
            d->set_disposable(current->subscribe(
                [observer](const Variant& value) { observer->on_next(value); },
                on_error_resume, on_resume, scheduler__
            ));
            return nullptr;
        };

        cancelable->set_disposable(scheduler->schedule(ACTION { return action(scheduler__, state, action); }));
        return std::make_shared<CompositeDisposable>(std::initializer_list<std::shared_ptr<DisposableBase>>{
            subscription, cancelable
        });

    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_ONERRORRESUMENEXT_H