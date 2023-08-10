#include "observable/observable.h"

#include "scheduler/currentthreadscheduler.h"
#include "disposable/serialdisposable.h"

using namespace rx::disposable;
using namespace rx::scheduler;

namespace rx::observable {

/* std::shared_ptr<Observable> Observable::catch_with_iterable(const std::shared_ptr<IterableBase>& sources) {
    auto sources_ = sources->iter();

    subscription_t subscribe = SUBSCRIBE(nullptr) {

        throw NotImplementedException();

        auto _scheduler = scheduler_ ? scheduler_ : CurrentThreadScheduler::singleton();

        auto subscription = std::make_shared<SerialDisposable>();
        auto cancelable = std::make_shared<SerialDisposable>();
        auto last_exception = std::make_shared<std::exception_ptr>();
        auto is_disposed = std::make_shared<bool>(false);

        action_t action = ACTION {
            on_error_t on_error = [=](const std::exception_ptr& exn) {
                *last_exception = exn;
                cancelable->set_disposable(_scheduler->schedule(action));
            };

            if (*is_disposed) {
                return nullptr;
            }

            Variant current;
            try { if (sources_->has_next()) {
                current = sources_->next();
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
            } catch (...) {
                observer->on_error(std::current_exception());
                return nullptr;
            }
            // else
            {
                auto d = std::make_shared<SingleAssignmentDisposable>();
                subscription->set_disposable(d);
                // d->set_disposable(current->subscribe // ?????? TODO

                // );
            }
            return nullptr;

        };

        return std::make_shared<Disposable>();
    };

    return Observable::get(SUBSCRIBE(nullptr) { throw NotImplementedException(); return std::make_shared<Disposable>(); });
} */

} // END namespace rx::observable