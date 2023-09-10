#include "scheduler/scheduler.h"

#include "disposable/disposable.h"

using namespace rx::disposable;

namespace rx::scheduler {

time_point_t Scheduler::now() {
    return basic::now<time_point_t>();
}

disposable_t Scheduler::invoke_action(const scheduled_action_t& action, const Variant& state) {
    auto res = action(getptr(), state);
    return res ? res : std::make_shared<Disposable>();
}

} // END namespace rx::scheduler