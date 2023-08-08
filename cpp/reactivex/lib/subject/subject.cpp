#include "subject/subject.h"

#include "subject/innersubscription.h"

namespace rx::subject {

void Subject::check_disposed() {
    if (this->is_disposed) {
        throw DisposedException();
    }
}

std::shared_ptr<DisposableBase> Subject::_subscribe_core(const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler) {
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
        if (!this->is_disposed) {
            this->observers.push_back(observer);
            return std::make_shared<InnerSubscription>(getptr(), observer);
        }

        if (this->exception) {
            observer->on_error(*(this->exception));
        }
        else {
            observer->on_completed();
        }
        return std::make_shared<Disposable>();
    }
}

void Subject::on_next(const Variant& item) {
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
    }
    Observer::on_next(item);
}

void Subject::_on_next_core(const Variant& item) {
    std::list<std::shared_ptr<ObserverBase>> observers;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
    }
    for (auto observer : observers) {
        observer->on_next(item);
    }
}

void Subject::on_error(const std::exception& e) {
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
    }
    Observer::on_error(e);
}

void Subject::_on_error_core(const std::exception& error) {
    std::list<std::shared_ptr<ObserverBase>> observers;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->observers.clear();
        this->exception = std::make_unique<std::exception>(error);
    }

    for (auto observer : observers) {
        observer->on_error(error);
    }
}

void Subject::on_completed() {
    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
    }
    Observer::on_completed();
}

void Subject::_on_completed_core() {
    std::list<std::shared_ptr<ObserverBase>> observers;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->observers.clear();
    }

    for (auto observer : observers) {
        observer->on_completed();
    }
}

void Subject::dispose() {
    std::lock_guard<RLock> guard(lock);
    this->is_disposed = true;
    this->observers.clear();
    this->exception = nullptr;
    Observer::dispose();
}

} // END namespace rx::subject