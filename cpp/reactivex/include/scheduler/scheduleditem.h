#ifndef RX_SCHEDULER_SCHEDULEDITEM_H
#define RX_SCHEDULER_SCHEDULEDITEM_H

#include <godot_cpp/variant/variant.hpp>

#include <memory>

#include "basic.h"
#include "abstract/scheduler.h"
#include "internal/time.h"
#include "disposable/singleassignmentdisposable.h"

using namespace godot;
using namespace rx::disposable;

namespace rx::scheduler {

struct scheduled_item_t {
public:
    std::shared_ptr<SchedulerBase> scheduler;
    Variant state;
    action_t action;
    time_point_t duetime;
    std::shared_ptr<SingleAssignmentDisposable> disposable;

    scheduled_item_t(const std::shared_ptr<SchedulerBase>& scheduler_, const Variant& state_, const action_t& action_, const time_point_t& duetime_) 
        : scheduler(scheduler_), state(state_), action(action_), duetime(duetime_), disposable(std::make_shared<SingleAssignmentDisposable>()) {}
    ~scheduled_item_t(){}

    void invoke() const;
    void cancel() const;
    bool is_cancelled() const;

    bool operator==(const scheduled_item_t& other) const;
    bool operator!=(const scheduled_item_t& other) const;
    bool operator<(const scheduled_item_t& other) const;
    bool operator>(const scheduled_item_t& other) const;
};

} // END namespace rx

#endif // RX_SCHEDULER_SCHEDULEDITEM_H