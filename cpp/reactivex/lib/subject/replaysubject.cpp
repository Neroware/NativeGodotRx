#include "subject/replaysubject.h"

#include "observer/scheduledobserver.h"

using namespace rx::observer;

namespace rx::subject {

disposable_t ReplaySubject::_subscribe_core(const observer_t& observer, const scheduler_t& scheduler
) {
    auto so = ScheduledObserver::get(this->scheduler, observer);
    auto subscription = std::make_shared<RemovableDisposable>(getptr(), so);

    {
        std::lock_guard<RLock> guard(lock);
        this->check_disposed();
        this->_trim(this->scheduler->now());
        this->observers.push_back(so);

        for (auto item : this->queue) {
            so->on_next(item.value);
        }

        if (this->exception) {
            so->on_error(this->exception);
        }
        else if (this->is_disposed) {
            so->on_completed();
        }
    }

    so->ensure_active();
    return subscription;
}

void ReplaySubject::_trim(const time_point_t& now) {
    while (this->queue.size() > this->buffer_size) {
        this->queue.pop_front();
    }

    while (!this->queue.empty() && (now - this->queue.front().interval) > this->window) {
        this->queue.pop_front();
    }
}

void ReplaySubject::_on_next_core(const Variant& value) {
    std::list<observer_t> observers;
    time_point_t now;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        now = this->scheduler->now();
        this->queue.push_back(queue_item_t{now, value});
        this->_trim(now);
    }

    for (auto observer : observers) {
        observer->on_next(value);
    }

    for (auto observer : observers) {
        std::static_pointer_cast<ScheduledObserver>(observer)->ensure_active();
    }
}

void ReplaySubject::_on_error_core(const std::exception_ptr& error) {
    std::list<observer_t> observers;
    time_point_t now;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->observers.clear();
        this->exception = error;
        now = this->scheduler->now();
        this->_trim(now);
    }

    for (auto observer : observers) {
        observer->on_error(error);
        std::static_pointer_cast<ScheduledObserver>(observer)->ensure_active();
    }
}

void ReplaySubject::_on_completed_core() {
    std::list<observer_t> observers;
    time_point_t now;
    {
        std::lock_guard<RLock> guard(lock);
        observers = this->observers;
        this->observers.clear();
        now = this->scheduler->now();
        this->_trim(now);
    }

    for (auto observer : observers) {
        observer->on_completed();
        std::static_pointer_cast<ScheduledObserver>(observer)->ensure_active();
    }
}

void ReplaySubject::dispose() {
    std::lock_guard<RLock> guard(lock);
    this->queue.clear();
    Subject::dispose();
}

} // END namespace rx::subject