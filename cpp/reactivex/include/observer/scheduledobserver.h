#ifndef RX_OBSERVER_SCHEDULEDOBSERVER_H
#define RX_OBSERVER_SCHEDULEDOBSERVER_H

#include "basic.h"
#include "internal/rlock.h"

#include "abstract/scheduler.h"

#include "observer/observer.h"
#include "disposable/serialdisposable.h"

#include <list>


namespace rx::observer {

class ScheduledObserver : public Observer {

public:
    std::shared_ptr<SchedulerBase> scheduler;
    std::shared_ptr<ObserverBase> observer;

    RLock lock;
    bool is_acquired = false;
    bool has_faulted = false;
    std::list<action_t> queue;
    SerialDisposable disposable;

    /* Note to self: list append is generally NOT thread safe */

protected:
    ScheduledObserver(const std::shared_ptr<SchedulerBase>& scheduler, const std::shared_ptr<ObserverBase>& observer) 
        : Observer(), scheduler(scheduler), observer(observer) {}
public:
    inline std::shared_ptr<ScheduledObserver> getptr() { return std::static_pointer_cast<ScheduledObserver>(Observer::getptr()); }
    inline static std::shared_ptr<ScheduledObserver> get(const std::shared_ptr<SchedulerBase>& scheduler, const std::shared_ptr<ObserverBase>& observer) {
        return std::shared_ptr<ScheduledObserver>(new ScheduledObserver(scheduler, observer));
    }
    ~ScheduledObserver() {}

protected:
    virtual void _on_next_core(const Variant& item) override;
    virtual void _on_error_core(const std::exception_ptr& error) override;
    virtual void _on_completed_core() override;

public:
    void ensure_active();
    void run(const std::shared_ptr<SchedulerBase>& scheduler, const Variant& state = Variant());
    void dispose() override;

};

} // END namespace rx::observer

#endif // RX_OBSERVER_SCHEDULEDOBSERVER_H