#ifndef RX_SCHEDULER_PERIODICSCHEDULER_H
#define RX_SCHEDULER_PERIODICSCHEDULER_H

#include "abstract/periodicscheduler.h"
#include "scheduler/scheduler.h"
#include "internal/time.h"

using namespace rx::abstract;

namespace rx::scheduler {

class PeriodicScheduler : public Scheduler, public PeriodicSchedulerBase {

public:
    PeriodicScheduler(){}
    ~PeriodicScheduler(){}
    inline std::shared_ptr<PeriodicScheduler> getptr() { return std::static_pointer_cast<PeriodicScheduler>(Scheduler::getptr()); }

    disposable_t schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;
};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_PERIODICSCHEDULER_H