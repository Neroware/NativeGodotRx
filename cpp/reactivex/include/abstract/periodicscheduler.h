#ifndef RX_ABSTRACT_PERIODICSCHEDULER_H
#define RX_ABSTRACT_PERIODICSCHEDULER_H

#include <godot_cpp/variant/variant.hpp>

#include "abstract/disposable.h"
#include "internal/time.h"

#include <memory>

using namespace godot;

namespace rx {

typedef std::function<Variant(const Variant&)> periodic_action_t;

namespace abstract {

class PeriodicSchedulerBase {

public:
    virtual std::shared_ptr<DisposableBase> schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) = 0;
};

} // END namespace abstract
} // END namespace rx

#endif // RX_ABSTRACT_PERIODICSCHEDULER_H