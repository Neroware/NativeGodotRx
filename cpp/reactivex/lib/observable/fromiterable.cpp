#include "observable/definitions.h"

#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

rx_observable_t obs::from_iterable_(const iterable_t& iterable, const scheduler_t& scheduler) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();
        auto _it = iterable->iter();
        auto disposed = std::make_shared<bool>(false);

        scheduled_action_t action = ACTION(scheduler__, state) {
            try {
                while (!(*disposed) && _it->has_next()) {
                    observer->on_next(_it->next());
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