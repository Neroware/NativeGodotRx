#include "subject/behaviorsubject.h"

#include "subject/innersubscription.h"

namespace rx::subject {

std::shared_ptr<DisposableBase> BehaviorSubject::_subscribe_core(const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler
) {
    std::exception_ptr ex;
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
        if (!this->is_stopped) {
            this->observers.push_back(observer);
            observer->on_next(this->value);
            return std::make_shared<InnerSubscription>(getptr(), observer);
        }

        ex = this->exception;
    }

    if (ex) {
        observer->on_error(ex);
    }
    else {
        observer->on_completed();
    }

    return std::make_shared<Disposable>();
}

void BehaviorSubject::_on_next_core(const Variant& value) {
    std::list<std::shared_ptr<ObserverBase>> observers;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->value = value;
    }

    for (auto observer : observers) {
        observer->on_next(value);
    }
}

void BehaviorSubject::dispose() {
    std::lock_guard<RLock> guard(lock);
    this->value = Variant();
    Subject::dispose();
}

} // END namespace rx::subject