#include "subject/asyncsubject.h"

#include "subject/innersubscription.h"

namespace rx::subject {

std::shared_ptr<DisposableBase> AsyncSubject::_subscribe_core(const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler
) {
    std::exception ex;
    bool failed, has_value;
    Variant value;
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
        if (!this->is_stopped) {
            this->observers.push_back(observer);
            return std::make_shared<InnerSubscription>(getptr(), observer);
        }

        failed = this->exception ? true : false;
        ex = failed ? *(this->exception) : ex;
        has_value = this->has_value;
        value = this->value;
    }

    if (failed) {
        observer->on_error(ex);
    }
    else if (has_value) {
        observer->on_next(value);
        observer->on_completed();
    }
    else {
        observer->on_completed();
    }

    return std::make_shared<Disposable>();
}

void AsyncSubject::_on_next_core(const Variant& item) {
    std::lock_guard<RLock> guard(lock);
    this->value = item;
    this->has_value = true;
}

void AsyncSubject::_on_completed_core() {
    std::list<std::shared_ptr<ObserverBase>> observers;
    bool has_value;
    Variant value;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->observers.clear();
        value = this->value;
        has_value = this->has_value;
    }

    if (has_value) {
        for (auto observer : observers) {
            observer->on_next(value);
            observer->on_completed();
        }
    }
    else {
        for (auto observer : observers) {
            observer->on_completed();
        }
    }
}

void AsyncSubject::dispose() {
    std::lock_guard<RLock> guard(lock);
    this->value = Variant();
    Subject::dispose();
}

} // END namespace rx::subject