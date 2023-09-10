#include "scheduler/trampolinescheduler.h"

namespace rx::scheduler {

std::shared_ptr<Trampoline> TrampolineScheduler::get_trampoline() {
    return this->_tramp;
}

disposable_t TrampolineScheduler::schedule(const scheduled_action_t& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

disposable_t TrampolineScheduler::schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state) {
    auto dt = std::max<time_delta_t>(DELTA_ZERO, duetime);
    return this->schedule_absolute(this->now() + dt, action, state);
}

disposable_t TrampolineScheduler::schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state) {
    if (duetime > this->now()) {
        UtilityFunctions::push_warning("Do not schedule blocking work!");
    }
    auto item = scheduled_item_t(getptr(), state, action, duetime);

    this->get_trampoline()->run(item);

    return item.disposable;
}
    
bool TrampolineScheduler::schedule_required() {
    return this->get_trampoline()->idle();
}

disposable_t TrampolineScheduler::ensure_trampoline(const scheduled_action_t& action) {
    if (this->schedule_required()) {
        return this->schedule(action);
    }
    return action(getptr(), Variant());
}

} // END namespace rx::scheduler