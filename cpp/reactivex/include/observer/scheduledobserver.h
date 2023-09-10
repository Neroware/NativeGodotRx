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
    scheduler_t scheduler;
    observer_t observer;

    RLock lock;
    bool is_acquired = false;
    bool has_faulted = false;
    std::list<scheduled_action_t> queue;
    SerialDisposable disposable;

    /* Note to self: list append is generally NOT thread safe */

protected:
    ScheduledObserver(const scheduler_t& scheduler, const observer_t& observer) 
        : Observer(), scheduler(scheduler), observer(observer) {}
public:
    inline std::shared_ptr<ScheduledObserver> getptr() { return std::static_pointer_cast<ScheduledObserver>(Observer::getptr()); }
    inline static std::shared_ptr<ScheduledObserver> get(const scheduler_t& scheduler, const observer_t& observer) {
        return std::shared_ptr<ScheduledObserver>(new ScheduledObserver(scheduler, observer));
    }
    ~ScheduledObserver() {}

protected:
    virtual void _on_next_core(const Variant& item) override;
    virtual void _on_error_core(const error_t& error) override;
    virtual void _on_completed_core() override;

public:
    void ensure_active();
    void run(const scheduler_t& scheduler, const Variant& state = Variant());
    void dispose() override;

};

} // END namespace rx::observer

#endif // RX_OBSERVER_SCHEDULEDOBSERVER_H