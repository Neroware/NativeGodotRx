#ifndef RX_SCHEDULER_IMMEDIATESCHEDULER_H
#define RX_SCHEDULER_IMMEDIATESCHEDULER_H

#include "scheduler/scheduler.h"

namespace rx::scheduler {

class ImmediateScheduler : public Scheduler {

protected:
    ImmediateScheduler(){}
public:
    ~ImmediateScheduler(){}
    inline std::shared_ptr<ImmediateScheduler> getptr() { return std::static_pointer_cast<ImmediateScheduler>(Scheduler::getptr()); }
    inline static std::shared_ptr<ImmediateScheduler> get() { return std::shared_ptr<ImmediateScheduler>(new ImmediateScheduler()); }

    static std::shared_ptr<ImmediateScheduler> singleton();

    disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
};

}

#endif // RX_SCHEDULER_IMMEDIATESCHEDULER_H