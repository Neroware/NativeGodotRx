#ifndef RX_SCHEDULER_EVENTLOOPSCHEDULER_H
#define RX_SCHEDULER_EVENTLOOPSCHEDULER_H

#include "abstract/startable.h"
#include "internal/concurrency.h"
#include "internal/lock.h"
#include "disposable/disposable.h"
#include "scheduler/periodicscheduler.h"
#include "scheduler/scheduleditem.h"

#include <condition_variable>
#include <list>
#include <deque>
#include <queue>

namespace rx::scheduler {

class EventLoopScheduler : public PeriodicScheduler, public DisposableBase {

private:
    bool _is_disposed = false;
    thread_factory_t _thread_factory;
    startable_t _thread;
    Lock _lock;
    std::condition_variable_any _condition;
    std::priority_queue<scheduled_item_t> _queue;
    std::list<scheduled_item_t> _ready_list;

    bool _exit_if_empty;

protected:
    EventLoopScheduler(const thread_factory_t& thread_factory = default_thread_factory, bool exit_if_empty = false) 
        : _thread_factory(thread_factory), _exit_if_empty(exit_if_empty) {}
public:
    ~EventLoopScheduler(){}
    inline static std::shared_ptr<EventLoopScheduler> get(const thread_factory_t& thread_factory = default_thread_factory, bool exit_if_empty = false) {
        return std::shared_ptr<EventLoopScheduler>(new EventLoopScheduler(thread_factory, exit_if_empty));
    }
    inline std::shared_ptr<EventLoopScheduler> getptr() { return std::static_pointer_cast<EventLoopScheduler>(PeriodicScheduler::getptr()); }

    disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;

private:
    bool _has_thread();
    void _ensure_thread();

public:
    void run();
    void dispose() override;

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_EVENTLOOPSCHEDULER_H