#ifndef RX_SCHEDULER_TRAMPOLINESCHEDULER_H
#define RX_SCHEDULER_TRAMPOLINESCHEDULER_H

#include "scheduler/scheduler.h"
#include "scheduler/trampoline.h"
#include "internal/time.h"

using namespace godot;

namespace rx::scheduler {

class TrampolineScheduler : public Scheduler, public std::enable_shared_from_this<TrampolineScheduler> {

private:
    std::shared_ptr<Trampoline> _tramp;
    inline std::shared_ptr<TrampolineScheduler> getptr() { return std::enable_shared_from_this<TrampolineScheduler>::shared_from_this(); }
protected:
    TrampolineScheduler(){}
public:
    inline static std::shared_ptr<TrampolineScheduler> get() { return std::shared_ptr<TrampolineScheduler>(new TrampolineScheduler()); }
    virtual std::shared_ptr<Trampoline> get_trampoline();
    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    
    bool schedule_required();
    std::shared_ptr<DisposableBase> ensure_trampoline(const action_t& action);
};

}; // END namespace rx::scheduler

#endif // RX_SCHEDULER_TRAMPOLINESCHEDULER_H