#ifndef RX_SCHEDULER_TIMEOUTSCHEDULER_H
#define RX_SCHEDULER_TIMEOUTSCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "scheduler/periodicscheduler.h"

namespace rx::scheduler {

class TimeoutScheduler : public PeriodicScheduler, public std::enable_shared_from_this<TimeoutScheduler> {

protected:
    TimeoutScheduler(){}
public:
    ~TimeoutScheduler(){}
    inline static std::shared_ptr<TimeoutScheduler> get() { return std::shared_ptr<TimeoutScheduler>(new TimeoutScheduler()); }
    inline std::shared_ptr<TimeoutScheduler> getptr() { return std::enable_shared_from_this<TimeoutScheduler>::shared_from_this(); }

    static std::shared_ptr<TimeoutScheduler> singleton();

    std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) override;
    std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) override;
};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_TIMEOUTSCHEDULER_H
