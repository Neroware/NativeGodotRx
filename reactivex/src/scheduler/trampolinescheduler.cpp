#include "trampolinescheduler.h"

Ref<TrampolineScheduler> TrampolineScheduler::Get() {
    return memnew(TrampolineScheduler);
}

void TrampolineScheduler::_bind_methods() {
    ClassDB::bind_static_method("TrampolineScheduler", D_METHOD("Get"), &TrampolineScheduler::Get);
    ClassDB::bind_method(D_METHOD("get_trampoline"), &TrampolineScheduler::get_trampoline);
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &TrampolineScheduler::schedule);
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &TrampolineScheduler::schedule_relative);
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &TrampolineScheduler::schedule_absolute);
    ClassDB::bind_method(D_METHOD("schedule_required"), &TrampolineScheduler::schedule_required);
    ClassDB::bind_method(D_METHOD("ensure_trampoline", "action"), &TrampolineScheduler::ensure_trampoline);
}

Ref<Trampoline> TrampolineScheduler::get_trampoline() {
    return this->_tramp;
}

Ref<DisposableBase> TrampolineScheduler::schedule(const Callable& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

Ref<DisposableBase> TrampolineScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    duetime = Ref<RelativeTime>(&std::max<RelativeTime>(**RelativeTime::delta_zero(), **to_timedelta(Variant(duetime))));
    return this->schedule_absolute(**this->now() + **duetime, action, state);
}

Ref<DisposableBase> TrampolineScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto dt = to_datetime(duetime);
    if (**dt > **this->now()) {
        UtilityFunctions::push_warning("Do not schedule blocking work!");
    }
    auto item = ScheduledItem::Get(Ref<Scheduler>(this), state, action, dt);

    this->get_trampoline()->run(item);
    
    return item->disposable;
}
    
bool TrampolineScheduler::schedule_required() {
    return this->get_trampoline()->idle();
}

Ref<Trampoline> TrampolineScheduler::ensure_trampoline(const Callable& action) {
    if (this->schedule_required()) {
        return this->schedule(action);
    }

    return action.callv(Array::make(Ref<Scheduler>(this), Variant()));
}