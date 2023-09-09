#ifndef RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H
#define RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H

#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "basic.h"
#include "internal/rlock.h"
#include "exception/exception.h"
#include "singleassignmentdisposable.h"

namespace rx::disposable {

class ScheduledDisposable : public DisposableBase, public std::enable_shared_from_this<ScheduledDisposable> {

public:
    scheduler_t scheduler;
    SingleAssignmentDisposable disposable;
    RLock lock;

protected:
    ScheduledDisposable(const scheduler_t& scheduler_, const disposable_t& disposable_) 
        : scheduler(scheduler_) {
            this->disposable.set_disposable(disposable_);
    }
public:
    ~ScheduledDisposable(){ this->dispose(); }
    inline static std::shared_ptr<ScheduledDisposable> get(const scheduler_t& scheduler_, const disposable_t& disposable_) {
        return std::shared_ptr<ScheduledDisposable>(new ScheduledDisposable(scheduler_, disposable_));
    }
    inline std::shared_ptr<ScheduledDisposable> getptr() { return shared_from_this(); }

    void dispose() override;
    bool is_disposed();
};

} // END namespace rx::disposable

#endif // RX_DISPOSABLE_SCHEDULEDDISPOSABLE_H