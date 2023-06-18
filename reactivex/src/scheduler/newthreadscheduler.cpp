#include "newthreadscheduler.h"

#include "internal/threadingevent.h"
#include "scheduler/eventloopscheduler.h"
#include "disposable/disposable.h"

void NewThreadScheduler::_bind_methods() {
    ClassDB::bind_static_method("NewThreadScheduler", D_METHOD("Get", "thread_factory"), &NewThreadScheduler::Get, DEFVAL(Lambda(VARIANT_FUN1(default_thread_factory))));
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &NewThreadScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &NewThreadScheduler::schedule_absolute, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &NewThreadScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_periodic", "period", "action", "state"), &NewThreadScheduler::schedule_periodic, DEFVAL(Variant()));
}

Ref<NewThreadScheduler> NewThreadScheduler::Get(const Callable& thread_factory) {
    return memnew(NewThreadScheduler(thread_factory));
}

Ref<DisposableBase> NewThreadScheduler::schedule(const Callable& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::Get(this->_thread_factory, true);
    return scheduler->schedule(action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    auto scheduler = EventLoopScheduler::Get(this->_thread_factory, true);
    return scheduler->schedule_relative(duetime, action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto dt = to_datetime(duetime);
    return this->schedule_relative(**dt - **(this->now()), action, state);
}

Ref<DisposableBase> NewThreadScheduler::schedule_periodic(Ref<RelativeTime> period, const Callable& action, const Variant& state) {

    double seconds = to_seconds(period);
    double timeout = seconds;
    auto disposed = ThreadingEvent::Get();
    Ref<NewThreadScheduler> self(this);

    Array nonlocals = Array::make(state, timeout);
    auto run = Lambda(VOID_FUN0([=](){
        auto _disposed = disposed; auto _self = self; auto _nonlocals = nonlocals;
        Variant _state = _nonlocals[0]; double _timeout = _nonlocals[1];
        while (true) {
            if (_timeout > 0.0) {
                _disposed->wait(_timeout);
            }
            if (_disposed->is_set()) {
                return;
            }

            auto time = _self->now();

            _state = action.callv(Array::make(_state));

            _timeout = seconds - (**(_self->now()) - **time)->to_sec();

            _nonlocals[0] = _state; _nonlocals[1] = _timeout;
        }
    }));

    auto thread_ = this->_thread_factory.callv(Array::make(run));
    if (auto tmp = CAST_OR_NULL(thread_, StartableBase)) {
        Ref<StartableBase> thread(tmp);
        thread->start();

        auto dispose = Lambda(VOID_FUN0([=](){
            auto _disposed = disposed; auto _thread = thread;
            _disposed->set_flag();
            _thread->await();
        }));

        return Disposable::Get(dispose);
    }

    throw BadArgumentException("Thread factory of EventLoopScheduler must return a StartableBase!");
}