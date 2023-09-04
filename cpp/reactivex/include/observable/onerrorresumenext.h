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

template<typename IterableT>
static std::shared_ptr<Observable> on_error_resume_next_(const IterableT& sources) {

    auto _end = sources.end();
    auto _it = std::make_shared<typename IterableT::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        
        auto scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
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
                scheduler__->schedule(RECURSIVE_ACTION_FWD(_action), RxError::wrap(state));
            };
            auto on_resume = [=]() {
                scheduler__->schedule(RECURSIVE_ACTION_FWD(_action));
            };
            
            d->set_disposable(current->subscribe(
                [observer](const Variant& value) { observer->on_next(value); },
                on_error_resume, on_resume, scheduler__
            ));
            return nullptr;
        };

        cancelable->set_disposable(scheduler->schedule(RECURSIVE_ACTION_FWD(action)));
        return std::make_shared<CompositeDisposable>(subscription, cancelable);

    };

    return Observable::get(subscribe);
}

template<typename... Args>
static std::shared_ptr<Observable> on_error_resume_next_(const Args&... sources) {
    std::vector<std::shared_ptr<Observable>> args = {sources...};
    return on_error_resume_next_(args);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_ONERRORRESUMENEXT_H