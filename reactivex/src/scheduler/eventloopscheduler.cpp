#include "eventloopscheduler.h"

#include <godot_cpp/variant/callable.hpp>

#include "exception/exception.h"
#include "internal/time.h"
#include "scheduler/scheduleditem.h"
#include "disposable/disposable.h"

#include "godotrx.h"

void EventLoopScheduler::_bind_methods() {
    ClassDB::bind_static_method("EventLoopScheduler", D_METHOD("Get", "thread_factory", "exit_if_empty"), &EventLoopScheduler::Get, DEFVAL(Lambda(VARIANT_FUN1(default_thread_factory))), DEFVAL(false));
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &EventLoopScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &EventLoopScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &EventLoopScheduler::schedule_absolute, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_periodic", "period", "action", "state"), &EventLoopScheduler::schedule_periodic, DEFVAL(Variant()));

    ClassDB::bind_method(D_METHOD("_has_thread"), &EventLoopScheduler::_has_thread);
    ClassDB::bind_method(D_METHOD("_ensure_thread"), &EventLoopScheduler::_ensure_thread);

    ClassDB::bind_method(D_METHOD("run"), &EventLoopScheduler::run);
    ClassDB::bind_method(D_METHOD("dispose"), &EventLoopScheduler::dispose);
}

Ref<EventLoopScheduler> EventLoopScheduler::Get(const Callable& thread_factory, bool exit_if_empty) {
    return memnew(EventLoopScheduler(thread_factory, exit_if_empty));
}

Ref<DisposableBase> EventLoopScheduler::schedule(const Callable& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

Ref<DisposableBase> EventLoopScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto delta_zero = RelativeTime::delta_zero();
    auto _duetime = Ref<RelativeTime>(&std::max(**delta_zero, **duetime));
    return this->schedule_absolute(**(this->now()) + **_duetime, action, state);
}

Ref<DisposableBase> EventLoopScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    if (this->_is_disposed) {
        throw DisposedException();
    }

    auto dt = duetime;
    auto si = ScheduledItem::Get(this, state, action, dt);
    {
        std::lock_guard<Lock> guard(**(this->_lock));
        if (**dt < **(this->now())) {
            this->_ready_list.append(si);
        }
        else {
            this->_queue.push(si);
        }
        this->_condition.notify_one();
        this->_ensure_thread();   
    }

    return Disposable::Get(Callable(*si, "cancel"));
}

Ref<DisposableBase> EventLoopScheduler::schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state) {
    if (this->_is_disposed) {
        throw DisposedException();
    }
    return PeriodicScheduler::schedule_periodic(period, action, state);
}

bool EventLoopScheduler::_has_thread() {
    std::lock_guard<Lock> guard(**(this->_lock));
    return !this->_is_disposed && !this->_thread.is_null();
}

void EventLoopScheduler::_ensure_thread() {
    if (this->_thread.is_null()) {
        auto thread_ = this->_thread_factory.callv(Array::make(Callable(this, "run")));
        if (auto thread = CAST_OR_NULL(thread_, StartableBase)) {
            this->_thread = Ref<StartableBase>(thread);
            thread->start();
        }
    }
}

void EventLoopScheduler::run() {
    std::deque<Ref<ScheduledItem>> ready;

    while (true) {
        {
            std::lock_guard<Lock> guard(**(this->_lock));
            if (this->_is_disposed) {
                return;
            }

            auto time = this->now();
            while (!this->_queue.empty()) {
                auto due = this->_queue.top()->duetime;
                while (!this->_ready_list.is_empty() && **due > **(Object::cast_to<ScheduledItem>(this->_ready_list[0])->duetime)) {
                    ready.push_back(this->_ready_list.pop_front());
                }
                if (**due > **time) {
                    break;
                }
                auto item_ = this->_queue.top();
                this->_queue.pop();
                ready.push_back(item_);
            }
            while (!this->_ready_list.is_empty()) {
                ready.push_back(this->_ready_list.pop_front());
            }
        }

        while (!ready.empty()) {
            auto item = ready.front();
            ready.pop_front();
            if (!item->is_cancelled()) {
                item->invoke();
            }
        }

        {
            std::lock_guard<Lock> guard(**(this->_lock));
            if (!this->_ready_list.is_empty()) {
                continue;
            }

            else if (!this->_queue.empty()) {
                auto time = this->now();
                auto item = this->_queue.top();
                double seconds = (**(item->duetime) - **time)->to_sec();
                if (seconds > 0) {
                    // UtilityFunctions::print("timeout: ", seconds);
                    this->_condition.wait_for(**_lock, std::chrono::duration<double>(seconds));
                }
            }

            else if (this->_exit_if_empty) {
                this->_thread = Ref<StartableBase>();
                return;
            }

            else {
                this->_condition.wait(**_lock);
            }
        }
    }
}

void EventLoopScheduler::dispose() {
    std::lock_guard<Lock> guard(**(this->_lock));
    if (!this->_is_disposed) {
        this->_is_disposed = true;
        this->_condition.notify_one();
    }
}