#ifndef RX_SCHEDULER_NEWTHREADSCHEDULER_H
#define RX_SCHEDULER_NEWTHREADSCHEDULER_H

#include "abstract/disposable.h"
#include "scheduler/periodicscheduler.h"
#include "internal/concurrency.h"

namespace rx::scheduler {

class NewThreadScheduler : public PeriodicScheduler {

private:
    thread_factory_t _thread_factory;

protected:
    NewThreadScheduler(const thread_factory_t& thread_factory = default_thread_factory) : _thread_factory(thread_factory) {}
public:
    ~NewThreadScheduler() {}
    inline std::shared_ptr<NewThreadScheduler> getptr() { return std::static_pointer_cast<NewThreadScheduler>(PeriodicScheduler::getptr()); }
    inline static std::shared_ptr<NewThreadScheduler> get(const thread_factory_t& thread_factory = default_thread_factory) {
        return std::shared_ptr<NewThreadScheduler>(new NewThreadScheduler(thread_factory));
    }

    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override;

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_NEWTHREADSCHEDULER_H