#include "scheduler/trampolinescheduler.h"

namespace rx::scheduler {

std::shared_ptr<Trampoline> TrampolineScheduler::get_trampoline() {
    return this->_tramp;
}

std::shared_ptr<DisposableBase> TrampolineScheduler::schedule(const action_t& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

std::shared_ptr<DisposableBase> TrampolineScheduler::schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state) {
    auto dt = std::max<time_delta_t>(DELTA_ZERO, duetime);
    return this->schedule_absolute(this->now() + dt, action, state);
}

std::shared_ptr<DisposableBase> TrampolineScheduler::schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state) {
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

std::shared_ptr<DisposableBase> TrampolineScheduler::ensure_trampoline(const action_t& action) {
    if (this->schedule_required()) {
        return this->schedule(action);
    }
    return action(getptr(), Variant());
}

} // END namespace rx::scheduler