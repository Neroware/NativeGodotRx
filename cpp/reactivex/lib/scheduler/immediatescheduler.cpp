#include "scheduler/immediatescheduler.h"

#include "exception/exception.h"
#include "godotrx.h"

namespace rx::scheduler {

std::shared_ptr<ImmediateScheduler> ImmediateScheduler::singleton() {
    return GDRX->ImmediateScheduler_;
}

std::shared_ptr<DisposableBase> ImmediateScheduler::schedule(const action_t& action, const Variant& state) {
    return this->invoke_action(action, state);
}

std::shared_ptr<DisposableBase> ImmediateScheduler::schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state) {
    auto _duetime = Scheduler::to_timedelta(duetime);
    if (_duetime > DELTA_ZERO) {
        throw WouldBlockException();
    }
    return this->invoke_action(action, state);
}

std::shared_ptr<DisposableBase> ImmediateScheduler::schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state) {
    auto _duetime = Scheduler::to_datetime(duetime);
    return this->schedule_relative(_duetime - this->now(), action, state);
}

} // END namespace rx::scheduler