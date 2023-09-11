#include "observable/definitions.h"

#include "disposable/serialdisposable.h"
#include "disposable/compositedisposable.h"

using namespace rx::scheduler;

namespace rx::observable {

rx_observable_t obs::on_error_resume_next_(const rx_observable_list_t& sources) {
    auto _it = std::make_shared<rx_observable_list_t::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        
        auto scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            rx_observable_t source;
            if (*_it != sources.end()) {
                source = **_it; (*_it)++;
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
                ONNEXT_FWD(observer), on_error_resume, 
                on_resume, scheduler__
            ));
            return nullptr;
        };

        cancelable->set_disposable(scheduler->schedule(RECURSIVE_ACTION_FWD(action)));
        return std::make_shared<CompositeDisposable>(subscription, cancelable);

    };

    return Observable::get(subscribe);
}

rx_observable_t obs::on_error_resume_next_(const RxList<stated_observable_factory_t>& sources) {
    auto _it = std::make_shared<RxList<stated_observable_factory_t>::const_iterator>(sources.begin());

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        
        auto scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            rx_observable_t source;
            if (*_it != sources.end()) {
                source = (**_it)(state); (*_it)++;
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
                ONNEXT_FWD(observer), on_error_resume, 
                on_resume, scheduler__
            ));
            return nullptr;
        };

        cancelable->set_disposable(scheduler->schedule(RECURSIVE_ACTION_FWD(action)));
        return std::make_shared<CompositeDisposable>(subscription, cancelable);

    };

    return Observable::get(subscribe);
}

rx_observable_t obs::on_error_resume_next_(const rx_observable_t& sources...) {
    return on_error_resume_next_({sources});
}

rx_observable_t obs::on_error_resume_next_(const stated_observable_factory_t& sources...) {
    return on_error_resume_next_({sources});
}

} // END namespace rx::observable