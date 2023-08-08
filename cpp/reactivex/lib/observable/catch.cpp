#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

std::shared_ptr<Observable> Observable::catch_with_iterable(const std::shared_ptr<IterableBase>& sources) {
    // auto sources_ = sources->iter();

    subscription_t subscribe = SUBSCRIBE(nullptr) {
        auto _scheduler = scheduler ? scheduler : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        std::shared_ptr<std::exception> last_exception;
        auto is_disposed = std::make_shared<bool>(false);

        // TODO Messed up exceptions, I think...
        throw NotImplementedException();

        return std::make_shared<Disposable>();
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable