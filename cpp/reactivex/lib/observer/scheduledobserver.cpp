#include "observer/scheduledobserver.h"

namespace rx::observer {

void ScheduledObserver::_on_next_core(const Variant& item) {
    auto self = getptr();

    scheduled_action_t action = [=](const scheduler_t scheduler, const Variant& state){
        self->observer->on_next(item);
        return nullptr;
    };

    std::lock_guard<RLock> guard(lock);
    this->queue.push_back(action);
}

void ScheduledObserver::_on_error_core(const error_t& error) {
    auto self = getptr();

    scheduled_action_t action = [=](const scheduler_t scheduler, const Variant& state){
        self->observer->on_error(error);
        return nullptr;
    };

    std::lock_guard<RLock> guard(lock);
    this->queue.push_back(action);
}

void ScheduledObserver::_on_completed_core() {
    auto self = getptr();

    scheduled_action_t action = [=](const scheduler_t scheduler, const Variant& state){
        self->observer->on_completed();
        return nullptr;
    };

    std::lock_guard<RLock> guard(lock);
    this->queue.push_back(action);
}

void ScheduledObserver::ensure_active() {
    bool is_owner = false;

    {
        std::lock_guard<RLock> guard(lock);
        if (!this->has_faulted && !this->queue.empty()) {
            is_owner = !this->is_acquired;
            this->is_acquired = true;
        }
    }

    if (is_owner) {
        auto self = getptr();
        scheduled_action_t run_ = [self](const scheduler_t& scheduler_, const Variant& state_) { 
            self->run(scheduler_, state_); 
            return nullptr; 
        };
        this->disposable.set_disposable(this->scheduler->schedule(run_));
    }
}

void ScheduledObserver::run(const scheduler_t& scheduler, const Variant& state) {
    auto parent = getptr();

    scheduled_action_t work;
    {
        std::lock_guard<RLock> guard(lock);
        if (!parent->queue.empty()) {
            work = parent->queue.front();
            parent->queue.pop_front();
        }
        else {
            parent->is_acquired = false;
            return;
        }
    }

    try {
        work(nullptr, Variant());
    }
    catch (...) {
        {
            std::lock_guard<RLock> guard(lock);
            parent->queue.clear();
            parent->has_faulted = true;
        }
        throw;
    }

    auto self = getptr();
    scheduled_action_t run_ = [self](const scheduler_t& scheduler_, const Variant& state_) { 
        self->run(scheduler_, state_); 
        return nullptr; 
    };
    this->scheduler->schedule(run_);
}

void ScheduledObserver::dispose() {
    Observer::dispose();
    this->disposable.dispose();
}

} // END rx::observer