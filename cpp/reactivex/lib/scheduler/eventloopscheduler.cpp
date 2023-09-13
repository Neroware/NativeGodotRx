#include "scheduler/eventloopscheduler.h"

#include "godotrx.h"

namespace rx::scheduler {

disposable_t EventLoopScheduler::schedule(const scheduled_action_t& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

disposable_t EventLoopScheduler::schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto _duetime = std::max(DELTA_ZERO, duetime);
    return this->schedule_absolute(this->now() + _duetime, action, state);
}

disposable_t EventLoopScheduler::schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state) {
    if (this->_is_disposed) {
        throw DisposedException();
    }

    auto dt = duetime;
    scheduled_item_t si(getptr(), state, action, dt);
    {
        std::lock_guard<Lock> guard(_lock);
        if (dt < this->now()) {
            this->_ready_list.push_back(si);
        }
        else {
            this->_queue.push(si);
        }
        this->_condition.notify_one();
        this->_ensure_thread();   
    }

    dispose_t on_dispose = [si](){
        si.cancel();
    };
    return std::make_shared<Disposable>(on_dispose);
}

disposable_t EventLoopScheduler::schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state) {
    if (this->_is_disposed) {
        throw DisposedException();
    }
    return PeriodicScheduler::schedule_periodic(period, action, state);
}

bool EventLoopScheduler::_has_thread() {
    std::lock_guard<Lock> guard(_lock);
    return !this->_is_disposed && this->_thread;
}

void EventLoopScheduler::_ensure_thread() {
    if (!this->_thread) {
        auto this_ptr = getptr();
        auto thread_manager = GDRX->THREAD_MANAGER;

        run_t _run = [this_ptr, thread_manager](){
            auto thread = this_ptr->_thread;
            this_ptr->run();
            thread_manager->finish(thread);
            return EXIT_SUCCESS; 
        };

        auto thread_ = this->_thread_factory(_run);
        if (thread_) {
            this->_thread = thread_;
            this->_thread->start();
        }
        else {
            throw BadArgumentException("Thread factory of EventLoopScheduler must return a StartableBase!");
        }
    }
}

void EventLoopScheduler::run() {
    std::deque<scheduled_item_t> ready;

    while (true) {
        {
            std::lock_guard<Lock> guard(_lock);
            if (this->_is_disposed) {
                return;
            }

            auto time = this->now();
            while (!this->_queue.empty()) {
                auto due = this->_queue.top().duetime;
                while (!this->_ready_list.empty() && due > this->_ready_list.front().duetime) {
                    auto _next_item = this->_ready_list.front();
                    this->_ready_list.pop_front();
                    ready.push_back(_next_item);
                }
                if (due > time) {
                    break;
                }
                auto item_ = this->_queue.top();
                this->_queue.pop();
                ready.push_back(item_);
            }
            while (!this->_ready_list.empty()) {
                auto _next_item = this->_ready_list.front();
                this->_ready_list.pop_front();
                ready.push_back(_next_item);
            }
        }

        while (!ready.empty()) {
            auto item = ready.front();
            ready.pop_front();
            if (!item.is_cancelled()) {
                item.invoke();
            }
        }

        {
            std::lock_guard<Lock> guard(_lock);
            if (!this->_ready_list.empty()) {
                continue;
            }

            else if (!this->_queue.empty()) {
                auto time = this->now();
                auto item = this->_queue.top();
                double seconds = Scheduler::to_seconds(item.duetime - time);
                if (seconds > 0) {
                    // UtilityFunctions::print("timeout: ", seconds);
                    this->_condition.wait_for(_lock, std::chrono::duration<double>(seconds));
                }
            }

            else if (this->_exit_if_empty) {
                this->_thread = nullptr;
                return;
            }

            else {
                this->_condition.wait(_lock);
            }
        }
    }
}

void EventLoopScheduler::dispose() {
    std::shared_ptr<StartableBase> thread;
    {
        std::lock_guard<Lock> guard(_lock);
        if (!this->_is_disposed) {
            // A priority queue does not free its contents when destroyed.
            // Also I think we need to free all members here otherwise there 
            // might be memory leaks (at least according to Godot).
            this->_is_disposed = true;
            while(!_queue.empty()) { _queue.pop(); }
            this->_ready_list.clear();
            this->_condition.notify_one();
            thread = this->_thread;
            this->_thread = nullptr;
        }
    }
    if (thread) { 
        thread->await();
    }
}

} // END namespace rx::scheduler