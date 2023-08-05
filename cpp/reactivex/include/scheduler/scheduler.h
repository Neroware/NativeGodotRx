#ifndef RX_SCHEDULER_SCHEDULER_H
#define RX_SCHEDULER_SCHEDULER_H

#include "abstract/scheduler.h"
#include "internal/time.h"

using namespace godot;
using namespace rx::abstract;

namespace rx::scheduler {

class Scheduler : public SchedulerBase, public std::enable_shared_from_this<Scheduler> {
private:

public:
    inline std::shared_ptr<Scheduler> getptr() { return shared_from_this(); }

    time_point_t now() override;
    std::shared_ptr<DisposableBase> invoke_action(const action_t& action, const Variant& state = Variant()) override;

    inline static double to_seconds(const double& value) { return value; }
    inline static double to_seconds(const time_delta_t& value) { return std::chrono::duration<double>(value).count(); }
    inline static double to_seconds(const time_point_t& value) { return std::chrono::duration<double>(value.time_since_epoch()).count(); }
    inline static time_delta_t to_timedelta(const double& value) { return DELTA_ZERO + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(value)); }
    inline static time_delta_t to_timedelta(const time_delta_t& value) { return value; }
    inline static time_delta_t to_timedelta(const time_point_t& value) { return value.time_since_epoch(); }
    inline static time_point_t to_datetime(const double& value) { return UTC_ZERO + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(value)); }
    inline static time_point_t to_datetime(const time_delta_t& value) { return UTC_ZERO + value; }
    inline static time_point_t to_datetime(const time_point_t& value) { return value; }

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_SCHEDULER_H