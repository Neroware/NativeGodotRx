#include "notification.h"

#include "scheduler/immediatescheduler.h"

void RxNotification::_bind_methods() {
    ClassDB::bind_method(D_METHOD("accept", "obs_or_onnext", "on_error", "on_completed"), &RxNotification::accept, DEFVAL(DEFAULT_ON_ERROR), DEFVAL(DEFAULT_ON_COMPLETED));
    ClassDB::bind_method(D_METHOD("_accept", "on_next", "on_error", "on_completed"), &RxNotification::_accept, DEFVAL(DEFAULT_ON_ERROR), DEFVAL(DEFAULT_ON_COMPLETED));
    ClassDB::bind_method(D_METHOD("_accept_observer", "obs"), &RxNotification::_accept_observer);
    ClassDB::bind_method(D_METHOD("to_observable", "scheduler"), &RxNotification::to_observable, DEFVAL(Ref<SchedulerBase>()));
}

void RxNotification::accept(const Variant& on_next, const Callable& on_error, const Callable& on_completed) {
    if (auto obs = CAST_OR_NULL(on_next, ObserverBase)) {
        return this->_accept_observer(obs);
    }
    return this->_accept(on_next, on_error, on_completed);
}

void RxNotification::_accept(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    throw NotImplementedException();
}

void RxNotification::_accept_observer(Ref<ObserverBase> observer) {
    throw NotImplementedException();
}

Ref<ObservableBase> RxNotification::to_observable(Ref<SchedulerBase> scheduler) {
    Ref<SchedulerBase> _scheduler = scheduler.is_null() ? REF_CAST(*ImmediateScheduler::singleton(), SchedulerBase) : scheduler;

    // TODO
    throw NotImplementedException();

    return Ref<ObservableBase>();

}


void OnNext::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_accept", "on_next", "on_error", "on_completed"), &OnNext::_accept, DEFVAL(DEFAULT_ON_ERROR), DEFVAL(DEFAULT_ON_COMPLETED));
    ClassDB::bind_method(D_METHOD("_accept_observer", "obs"), &OnNext::_accept_observer);
}
void OnNext::_accept(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    on_next.callv(Array::make(this->value));
}
void OnNext::_accept_observer(Ref<ObserverBase> observer) {
    observer->on_next(this->value);
}
String OnNext::_to_string() const {
    return "OnNext(" + UtilityFunctions::str(this->value) + ")";
}


void OnError::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_accept", "on_next", "on_error", "on_completed"), &OnError::_accept, DEFVAL(DEFAULT_ON_ERROR), DEFVAL(DEFAULT_ON_COMPLETED));
    ClassDB::bind_method(D_METHOD("_accept_observer", "obs"), &OnError::_accept_observer);
}
void OnError::_accept(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    on_error.callv(Array::make(this->exception));
}
void OnError::_accept_observer(Ref<ObserverBase> observer) {
    observer->on_error(this->exception);
}
String OnError::_to_string() const {
    return "OnError(" + UtilityFunctions::str(this->exception) + ")";
}


void OnCompleted::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_accept", "on_next", "on_error", "on_completed"), &OnCompleted::_accept, DEFVAL(DEFAULT_ON_ERROR), DEFVAL(DEFAULT_ON_COMPLETED));
    ClassDB::bind_method(D_METHOD("_accept_observer", "obs"), &OnCompleted::_accept_observer);
}
void OnCompleted::_accept(const Callable& on_next, const Callable& on_error, const Callable& on_completed) {
    on_completed.callv(Array());
}
void OnCompleted::_accept_observer(Ref<ObserverBase> observer) {
    observer->on_completed();
}
String OnCompleted::_to_string() const {
    return "OnCompleted()";
}


// TODO static inline Ref<Observer>
static Ref<ObserverBase> from_notifier(const Callable& handler) {
    auto _on_next = Lambda(VOID_FUN1([=](const Variant& value){
        handler.callv(Array::make(Ref<OnNext>(memnew(OnNext(value)))));
    }));

    auto _on_error = Lambda(VOID_FUN1([=](Ref<RxError> error){
        handler.callv(Array::make(Ref<OnError>(memnew(OnError(error)))));
    }));

    auto _on_completed = Lambda(VOID_FUN0([=](){
        handler.callv(Array::make(Ref<OnCompleted>(memnew(OnCompleted))));
    }));

    // TODO return Observer::Get(_on_next, _on_error, _on_completed);
    return Ref<ObserverBase>();
}