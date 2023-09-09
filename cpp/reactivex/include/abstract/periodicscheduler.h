#ifndef RX_ABSTRACT_PERIODICSCHEDULER_H
#define RX_ABSTRACT_PERIODICSCHEDULER_H

#include "internal/time.h"
#include "typing.h"

using namespace godot;

namespace rx::abstract {

class PeriodicSchedulerBase {

public:
    virtual disposable_t schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) = 0;
};

} // END namespace rx::abstract

#endif // RX_ABSTRACT_PERIODICSCHEDULER_H