#ifndef RX_SCHEDULER_IMMEDIATESCHEDULER_H
#define RX_SCHEDULER_IMMEDIATESCHEDULER_H

#include "scheduler/scheduler.h"

namespace rx::scheduler {

class ImmediateScheduler : public Scheduler {

protected:
    ImmediateScheduler(){}
public:
    ~ImmediateScheduler(){}
    inline static std::shared_ptr<ImmediateScheduler> get() { return std::shared_ptr<ImmediateScheduler>(new ImmediateScheduler()); }

    static std::shared_ptr<ImmediateScheduler> singleton();

    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
};

}

#endif // RX_SCHEDULER_IMMEDIATESCHEDULER_H