#include "timeoutscheduler.h"

#include "godotrx.h"

#include "disposable/disposable.h"
#include "disposable/singleassignmentdisposable.h"
#include "disposable/compositedisposable.h"
#include "internal/lambda.h"

#include <chrono>
#include <memory>
#include <thread>


void TimeoutScheduler::_bind_methods() {
    ClassDB::bind_static_method("TimeoutScheduler", D_METHOD("Get"), &TimeoutScheduler::Get);
    ClassDB::bind_static_method("TimeoutScheduler", D_METHOD("singleton"), &TimeoutScheduler::singleton);
    ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &TimeoutScheduler::schedule, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &TimeoutScheduler::schedule_relative, DEFVAL(Variant()));
    ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &TimeoutScheduler::schedule_absolute, DEFVAL(Variant()));
}

Ref<TimeoutScheduler> TimeoutScheduler::Get() {
    return memnew(TimeoutScheduler);
}

Ref<TimeoutScheduler> TimeoutScheduler::singleton() {
    return GDRX->TimeoutScheduler_;
}

Ref<DisposableBase> TimeoutScheduler::schedule(const Callable& action, const Variant& state) {
    auto sad = SingleAssignmentDisposable::Get();

    auto cancel = std::make_shared<bool>(false);
    auto self = Ref<TimeoutScheduler>(this);
    auto interval = [=]() {
        auto _sad = sad; auto _self = self;
        std::this_thread::sleep_for(std::chrono::seconds(0));
        if (!(*cancel)) _sad->set_disposable(_self->invoke_action(action, state));
    };

    std::thread timer(interval);
    timer.detach();

    auto dispose = Lambda(VOID_FUN0([cancel]() {
        *cancel = true;
    }));

    return CompositeDisposable::Get(Array::make(sad, Disposable::Get(dispose)));
}

Ref<DisposableBase> TimeoutScheduler::schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state) {
    double seconds = to_seconds(duetime);
    if (seconds < 0.0) {
        return this->schedule(action, state);
    }

    auto sad = SingleAssignmentDisposable::Get();

    auto cancel = std::make_shared<bool>(false);
    auto self = Ref<TimeoutScheduler>(this);
    auto interval = [=]() {
        auto _sad = sad; auto _self = self;
        std::this_thread::sleep_for(
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::duration<double>(seconds))
        );
        if (!(*cancel)) _sad->set_disposable(_self->invoke_action(action, state));
    };

    std::thread timer(interval);
    timer.detach();

    auto dispose = Lambda(VOID_FUN0([cancel]() {
        *cancel = true;
    }));

    return CompositeDisposable::Get(Array::make(sad, Disposable::Get(dispose)));
}

Ref<DisposableBase> TimeoutScheduler::schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state) {
    auto t = to_datetime(duetime);
    return this->schedule_relative(**t - **(this->now()), action, state);
}