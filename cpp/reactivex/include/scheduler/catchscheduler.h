#ifndef RX_SCHEDULER_CATCHSCHEDULER_H
#define RX_SCHEDULER_CATCHSCHEDULER_H

#include "scheduler/periodicscheduler.h"
#include "internal/time.h"

using namespace godot;

namespace rx::scheduler {

class CatchScheduler : public PeriodicScheduler, public std::enable_shared_from_this<CatchScheduler> {

private:
    std::shared_ptr<SchedulerBase> _scheduler;
    handler_t _handler;
    std::shared_ptr<SchedulerBase> _recursive_original;
    std::shared_ptr<CatchScheduler> _recursive_wrapper;

protected:
    CatchScheduler(const std::shared_ptr<SchedulerBase>& scheduler, const handler_t& handler) 
        : _scheduler(scheduler), _handler(handler) {}
public:
    ~CatchScheduler() {}
    inline static std::shared_ptr<CatchScheduler> get(const std::shared_ptr<SchedulerBase>& scheduler, const handler_t& handler) {
        return std::shared_ptr<CatchScheduler>(new CatchScheduler(scheduler, handler));
    }
    inline std::shared_ptr<CatchScheduler> getptr() { return std::enable_shared_from_this<CatchScheduler>::shared_from_this(); }

    time_point_t now() override;
    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;

private:
    std::shared_ptr<CatchScheduler> _clone(const std::shared_ptr<SchedulerBase>& scheduler) const;
    action_t _wrap(const action_t& action);
    std::shared_ptr<CatchScheduler> _get_recursive_wrapper(const std::shared_ptr<SchedulerBase>& scheduler);

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_CATCHSCHEDULER_H