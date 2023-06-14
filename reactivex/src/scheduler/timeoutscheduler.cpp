#include "timeoutscheduler.h"

#include "godotrx.h"

#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"

#include <chrono>
#include <thread>


void TimeoutScheduler::_bind_methods() {

}

Ref<TimeoutScheduler> TimeoutScheduler::Get() {
    return memnew(TimeoutScheduler);
}

Ref<TimeoutScheduler> TimeoutScheduler::singleton() {
    return GDRX->TimeoutScheduler_;
}

Ref<DisposableBase> TimeoutScheduler::schedule(const Callable& action, const Variant& state) {
    auto sad = SingleAssignmentDisposable::Get();

    bool cancel = false;
    auto self = Ref<TimeoutScheduler>(this);
    auto interval = [&cancel, self, action, state, sad]() {
        auto _sad = sad; auto _self = self;
        std::this_thread::sleep_for(std::chrono::seconds(0));
        if (!cancel) _sad->set_disposable(_self->invoke_action(action, state));
    };

    std::thread timer(interval);
    timer.detach();

    auto dispose = [&cancel]() {
        cancel = true;
    };

    return Disposable::Get();
}

Ref<DisposableBase> TimeoutScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    return Disposable::Get();
}

Ref<DisposableBase> TimeoutScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    return Disposable::Get();
}