#ifndef RX_SCHEDULER_PERIODICSCHEDULER_H
#define RX_SCHEDULER_PERIODICSCHEDULER_H

#include "abstract/periodicscheduler.h"
#include "scheduler/scheduler.h"
#include "internal/time.h"

using namespace rx::abstract;

namespace rx::scheduler {

class PeriodicScheduler : public Scheduler, public PeriodicSchedulerBase, public std::enable_shared_from_this<PeriodicScheduler> {

public:
    PeriodicScheduler(){}
    ~PeriodicScheduler(){}
    inline std::shared_ptr<PeriodicScheduler> getptr() { return std::enable_shared_from_this<PeriodicScheduler>::shared_from_this(); }

    std::shared_ptr<DisposableBase> schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;
};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_PERIODICSCHEDULER_H