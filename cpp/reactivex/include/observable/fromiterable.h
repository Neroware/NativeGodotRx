#ifndef RX_OBSERVABLE_FROMITERABLE_H
#define RX_OBSERVABLE_FROMITERABLE_H

#include "observable/observable.h"

#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

static std::shared_ptr<Observable> from_iterable_(const std::shared_ptr<IterableBase>& iterable, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {

    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();
        auto iterator = iterable->iter();
        auto disposed = std::make_shared<bool>(false);

        action_t action = ACTION {
            try {
                while (!(*disposed) && iterator->has_next()) {
                    observer->on_next(iterator->next());
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

        dispose_t dispose = [&]() {
            *disposed = true;
        };

        auto disp = std::make_shared<Disposable>(dispose);
        return std::make_shared<CompositeDisposable>(std::initializer_list<std::shared_ptr<DisposableBase>>{
            _scheduler->schedule(action), disp
        });
    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_FROMITERABLE_H