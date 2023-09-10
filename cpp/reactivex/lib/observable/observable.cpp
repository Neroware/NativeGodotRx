#include "observable/observable.h"

#include "observer/autodetachobserver.h"
#include "scheduler/currentthreadscheduler.h"

using namespace rx::observer;
using namespace rx::scheduler;

namespace rx::observable {

disposable_t Observable::_subscribe_core(const observer_t& observer, const scheduler_t& scheduler) {
    return this->_subscribe(observer, scheduler);
}

disposable_t Observable::subscribe(const observer_t& observer, const scheduler_t& scheduler) {
    on_next_t on_next_ = [observer](const Variant& item) { observer->on_next(item); };
    on_error_t on_error_ = [observer](const std::exception_ptr& e) { observer->on_error(e); };
    on_completed_t on_completed_ = [observer]() { observer->on_completed(); };
    return subscribe(on_next_, on_error_, on_completed_, scheduler);
}

disposable_t Observable::subscribe(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed, const scheduler_t& scheduler) {
    auto self = getptr();
    
    auto auto_detach_observer = std::make_shared<AutoDetachObserver>(on_next, on_error, on_completed);

    auto fix_subscriber = [=](const disposable_t& subscriber) -> disposable_t {
        if (!subscriber) {
            return std::make_shared<Disposable>();
        }
        return subscriber;
    };

    auto set_disposable = [=](const scheduler_t& __ = nullptr, const Variant& ___ = Variant()) -> disposable_t {
        std::shared_ptr<DisposableBase> subscriber;
        try {
            subscriber = self->_subscribe_core(auto_detach_observer, scheduler);
        }
        catch(...) {
            if (!auto_detach_observer->fail(std::current_exception())) {
                throw;
            }
            return nullptr;
        }
        auto_detach_observer->set_disposable(fix_subscriber(subscriber));
        return nullptr;
    };

    auto cts = CurrentThreadScheduler::singleton();
    if (cts->schedule_required()) {
        cts->schedule(set_disposable);
    }
    else {
        set_disposable();
    }

    dispose_t dispose = [auto_detach_observer]() {
        auto_detach_observer->dispose();
    };
    return std::make_shared<Disposable>(dispose);
}

} // END namespace rx::observable