#ifndef RX_OBSERVABLE_FROMITERABLE_H
#define RX_OBSERVABLE_FROMITERABLE_H

#include "observable/observable.h"

#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

template<typename IterableT>
static std::shared_ptr<Observable> from_iterable_(const IterableT& iterable, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();
        auto _it = std::make_shared<typename IterableT::const_iterator>(iterable.begin());
        auto _end = iterable.end();
        auto disposed = std::make_shared<bool>(false);

        action_t action = ACTION(scheduler__, state) {
            try {
                while (!(*disposed) && *_it != _end) {
                    observer->on_next(**_it);
                    ++(*_it);
                }
                if (!(*disposed)) {
                    observer->on_completed();
                }
            }
            catch(...) {
                observer->on_error(std::current_exception());
            }
            return nullptr;
        };

        dispose_t dispose = [disposed]() {
            *disposed = true;
        };

        auto disp = std::make_shared<Disposable>(dispose);
        return std::make_shared<CompositeDisposable>(
            _scheduler->schedule(action), disp
        );
    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_FROMITERABLE_H