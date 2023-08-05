#ifndef RX_SCHEDULER_CURRENTTHREADSCHEDULER_H
#define RX_SCHEDULER_CURRENTTHREADSCHEDULER_H

#include "internal/weakkeydictionary.h"
#include "internal/thread.h"
#include "scheduler/trampolinescheduler.h"

#include <string>

using namespace godot;

namespace rx::scheduler {

class CurrentThreadScheduler : public TrampolineScheduler, public std::enable_shared_from_this<CurrentThreadScheduler> {

private:
    weakkey_dictionary<variant_key_t, std::shared_ptr<Trampoline>> _tramps;

protected:
    CurrentThreadScheduler(){}
public:
    ~CurrentThreadScheduler(){}
    inline static std::shared_ptr<CurrentThreadScheduler> get() { return std::shared_ptr<CurrentThreadScheduler>(new CurrentThreadScheduler()); }
    inline std::shared_ptr<CurrentThreadScheduler> getptr() { return std::enable_shared_from_this<CurrentThreadScheduler>::shared_from_this(); }

    static std::shared_ptr<CurrentThreadScheduler> singleton();
    std::shared_ptr<Trampoline> get_trampoline() override;

}; // END CurrentThreadScheduler


class _CurrentThreadScheduler_Local {

private:
    weakkey_dictionary<variant_key_t, std::shared_ptr<Trampoline>> _tramp;

public:
    _CurrentThreadScheduler_Local(){}
    ~_CurrentThreadScheduler_Local(){}
    inline static std::shared_ptr<_CurrentThreadScheduler_Local> get() {
        return std::make_shared<_CurrentThreadScheduler_Local>();
    }

    std::shared_ptr<Trampoline> get_trampoline();

}; // END _CurrentThreadScheduler_Local


class CurrentThreadSchedulerSingleton : public CurrentThreadScheduler, public std::enable_shared_from_this<CurrentThreadSchedulerSingleton> {

protected:
    CurrentThreadSchedulerSingleton(){}
public:
    ~CurrentThreadSchedulerSingleton(){}
    inline static std::shared_ptr<CurrentThreadSchedulerSingleton> get() { return std::shared_ptr<CurrentThreadSchedulerSingleton>(new CurrentThreadSchedulerSingleton()); }
    inline std::shared_ptr<CurrentThreadSchedulerSingleton> getptr() { return std::enable_shared_from_this<CurrentThreadSchedulerSingleton>::shared_from_this(); }

    std::shared_ptr<Trampoline> get_trampoline() override;

}; // END CurrentThreadSchedulerSingleton

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_CURRENTTHREADSCHEDULER_H