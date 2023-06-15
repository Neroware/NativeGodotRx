#include "eventloopscheduler.h"

#include "exception/exception.h"
#include "internal/time.h"
#include "scheduler/scheduleditem.h"

#include "godotrx.h"

void EventLoopScheduler::_bind_methods() {

}

Ref<EventLoopScheduler> Get(const Callable& thread_factory = Lambda(VARIANT_FUN1(default_thread_factory)), bool exit_if_empty = false) {
    return memnew(EventLoopScheduler(thread_factory, exit_if_empty));
}

Ref<DisposableBase> EventLoopScheduler::schedule(const Callable& action, const Variant& state) {
    return this->schedule_absolute(this->now(), action, state);
}

Ref<DisposableBase> EventLoopScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto delta_zero = RelativeTime::delta_zero();
    auto _duetime = Ref<RelativeTime>(&std::max(**delta_zero, **duetime));
    return this->schedule_absolute(**(this->now()) + **_duetime, action, state);
}

Ref<DisposableBase> EventLoopScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    if (this->_is_disposed) {
        throw DisposedException();
    }

    auto dt = duetime;
    auto si = ScheduledItem::Get(this, state, action, dt);

    {
        std::lock_guard<Lock> guard(**(this->_lock));
        // TODO
    }
    return Ref<DisposableBase>();
}