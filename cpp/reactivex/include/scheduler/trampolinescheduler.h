#ifndef RX_SCHEDULER_TRAMPOLINESCHEDULER_H
#define RX_SCHEDULER_TRAMPOLINESCHEDULER_H

#include "scheduler/scheduler.h"
#include "scheduler/trampoline.h"
#include "internal/time.h"

using namespace godot;

namespace rx::scheduler {

class TrampolineScheduler : public Scheduler {

private:
    const std::shared_ptr<Trampoline> _tramp = std::make_shared<Trampoline>();
protected:
    TrampolineScheduler() {}
public:
    inline static std::shared_ptr<TrampolineScheduler> get() { return std::shared_ptr<TrampolineScheduler>(new TrampolineScheduler()); }
    inline std::shared_ptr<TrampolineScheduler> getptr() { return std::static_pointer_cast<TrampolineScheduler>(Scheduler::getptr()); }
    virtual std::shared_ptr<Trampoline> get_trampoline();
    disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    
    bool schedule_required();
    disposable_t ensure_trampoline(const scheduled_action_t& action);
};

}; // END namespace rx::scheduler

#endif // RX_SCHEDULER_TRAMPOLINESCHEDULER_H