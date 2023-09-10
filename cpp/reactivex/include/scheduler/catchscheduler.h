#ifndef RX_SCHEDULER_CATCHSCHEDULER_H
#define RX_SCHEDULER_CATCHSCHEDULER_H

#include "scheduler/periodicscheduler.h"
#include "internal/time.h"

using namespace godot;

namespace rx::scheduler {

class CatchScheduler : public PeriodicScheduler {

private:
    scheduler_t _scheduler;
    handler_t _handler;
    scheduler_t _recursive_original;
    std::shared_ptr<CatchScheduler> _recursive_wrapper;

protected:
    CatchScheduler(const scheduler_t& scheduler, const handler_t& handler) 
        : _scheduler(scheduler), _handler(handler) {}
public:
    ~CatchScheduler() {}
    inline static std::shared_ptr<CatchScheduler> get(const scheduler_t& scheduler, const handler_t& handler) {
        return std::shared_ptr<CatchScheduler>(new CatchScheduler(scheduler, handler));
    }
    inline std::shared_ptr<CatchScheduler> getptr() { return std::static_pointer_cast<CatchScheduler>(PeriodicScheduler::getptr()); }

    time_point_t now() override;
    disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;

private:
    std::shared_ptr<CatchScheduler> _clone(const scheduler_t& scheduler) const;
    scheduled_action_t _wrap(const scheduled_action_t& action);
    std::shared_ptr<CatchScheduler> _get_recursive_wrapper(const scheduler_t& scheduler);

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_CATCHSCHEDULER_H