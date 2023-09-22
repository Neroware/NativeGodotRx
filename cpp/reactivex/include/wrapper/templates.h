#ifndef RX_WRAPPER_TEMPLATES_H
#define RX_WRAPPER_TEMPLATES_H

#include "typing.h"
#include "internal/utils.h"
#include "basic.h"

#include "internal/time.h"

#define RX_TEMPLATE_CALL(method, self, ...) _get_template_method(self, #method).callv(Array::make(__VA_ARGS__))

namespace rx {

static Callable _get_template_method(const Variant& target, const StringName& method) {
    bool r_valid = true;
    Callable res = target.get_named(method, r_valid);
    if (!r_valid) {
        throw NotImplementedException("Template lacks requested method");
    }
    return res;
}
static bool _has_template_method(const Variant& target, const StringName& method) {
    bool r_valid = true;
    target.get_named(method, r_valid);
    return r_valid;
}

namespace wrappers {

class RxOnNext : public RefCounted {
    GDCLASS(RxOnNext, RefCounted)

private:
    on_next_t on_next;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "value"), &RxOnNext::invoke);
        ClassDB::bind_method(D_METHOD("is_null"), &RxOnNext::is_null);
    }
public:
    RxOnNext() : on_next(DEFAULT_ON_NEXT) {}
    RxOnNext(const on_next_t& on_next_) : on_next(on_next_) {}
    inline void invoke(const Variant& item) { this->on_next(item); }
    inline bool is_null() { return !(this->on_next); }
};

class RxOnError : public RefCounted {
    GDCLASS(RxOnError, RefCounted)

private:
    on_error_t on_error;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "error"), &RxOnError::invoke);
        ClassDB::bind_method(D_METHOD("is_null"), &RxOnError::is_null);
    }
public:
    RxOnError() : on_error(DEFAULT_ON_ERROR) {}
    RxOnError(const on_error_t& on_error_) : on_error(on_error_) {}
    inline void invoke(Ref<RxError> error) { this->on_error(RxError::unwrap(error)); }
    inline bool is_null() { return !(this->on_error); }
};

class RxOnCompleted : public RefCounted {
    GDCLASS(RxOnCompleted, RefCounted)

private:
    on_completed_t on_completed;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke"), &RxOnCompleted::invoke);
        ClassDB::bind_method(D_METHOD("is_null"), &RxOnCompleted::is_null);
    }
public:
    RxOnCompleted() {}
    RxOnCompleted(const on_completed_t& on_completed_) : on_completed(on_completed_) {}
    inline void invoke() { this->on_completed(); }
    inline bool is_null() { return !(this->on_completed); }
};

class RxAction : public RefCounted {
    GDCLASS(RxAction, RefCounted)

private:
    scheduled_action_t action;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "scheduler", "state"), &RxAction::invoke, DEFVAL(VNULL));
        ClassDB::bind_method(D_METHOD("is_null"), &RxAction::is_null);
    }
public:
    RxAction() {}
    RxAction(const scheduled_action_t& action_) : action(action_) {}
    inline Ref<RxDisposableBase> invoke(Ref<RxSchedulerBase> scheduler, const Variant& state = VNULL) { return this->action(scheduler, state); }
    inline bool is_null() { return !(this->action); }
};

class RxPeriodicAction : public RefCounted {
    GDCLASS(RxPeriodicAction, RefCounted)

private:
    periodic_action_t action;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "state"), &RxPeriodicAction::invoke, DEFVAL(VNULL));
        ClassDB::bind_method(D_METHOD("is_null"), &RxPeriodicAction::is_null);
    }
public:
    RxPeriodicAction() {}
    RxPeriodicAction(const periodic_action_t& action_) : action(action_) {}
    inline Variant invoke(const Variant& state = VNULL) { return this->action(state); }
    inline bool is_null() { return !(this->action); }
};


class RxSchedulerTemplate_ : public RefCounted {
    GDCLASS(RxSchedulerTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _now);
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _invoke_action);
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _schedule);
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _schedule_relative);
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _schedule_absolute);
        BIND_VIRTUAL_METHOD(RxSchedulerTemplate_, _classname);
    }
public:
    virtual Ref<AbsoluteTime> _now(Ref<RxSchedulerBase> self) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _invoke_action(Ref<RxSchedulerBase> self, Ref<RxAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _schedule(Ref<RxSchedulerBase> self, Ref<RxAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _schedule_relative(Ref<RxSchedulerBase> self, Ref<RelativeTime> duetime, Ref<RxAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _schedule_absolute(Ref<RxSchedulerBase> self, Ref<AbsoluteTime> duetime, Ref<RxAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateScheduler";
    }
};

class RxPeriodicSchedulerTemplate_ : public RxSchedulerTemplate_ {
    GDCLASS(RxPeriodicSchedulerTemplate_, RxSchedulerTemplate_)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxPeriodicSchedulerTemplate_, _schedule_periodic);
    }
public:
    virtual Ref<RxDisposableBase> _schedule_periodic(Ref<RxPeriodicSchedulerBase> self, Ref<RelativeTime> duetime, Ref<RxPeriodicAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
};

class RxGodotSignalSchedulerTemplate_ : public RxSchedulerTemplate_ {
    GDCLASS(RxGodotSignalSchedulerTemplate_, RxSchedulerTemplate_)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxGodotSignalSchedulerTemplate_, _schedule_signal);
    }
public:
    virtual Ref<RxDisposableBase> _schedule_signal(Ref<RxGodotSignalSchedulerBase> self, Object* owner, const StringName& signal, Ref<RxPeriodicAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
};

class RxDisposableTemplate_ : public RefCounted {
    GDCLASS(RxDisposableTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxDisposableTemplate_, _dispose);
        BIND_VIRTUAL_METHOD(RxDisposableTemplate_, _classname);
    }
public:
    virtual void _dispose(Ref<RxDisposableBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateDisposable";
    }
};

class RxIterableTemplate_ : public RefCounted {
    GDCLASS(RxIterableTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxIterableTemplate_, _iter);
        BIND_VIRTUAL_METHOD(RxIterableTemplate_, _classname);
    }
public:
    virtual Ref<RxIteratorBase> _iter(Ref<RxIterableBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateIterable";
    }
};

class RxIteratorTemplate_ : public RefCounted {
    GDCLASS(RxIteratorTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxIteratorTemplate_, _has_next);
        BIND_VIRTUAL_METHOD(RxIteratorTemplate_, _next);
        BIND_VIRTUAL_METHOD(RxIteratorTemplate_, _classname);
    }
public:
    virtual bool _has_next(Ref<RxIterableBase> self) {
        throw NotImplementedException();
    }
    virtual Variant _next(Ref<RxIterableBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateIterator";
    }
};

class RxLockTemplate_ : public RefCounted {
    GDCLASS(RxLockTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxLockTemplate_, _try_lock);
        BIND_VIRTUAL_METHOD(RxLockTemplate_, _lock);
        BIND_VIRTUAL_METHOD(RxLockTemplate_, _unlock);
        BIND_VIRTUAL_METHOD(RxLockTemplate_, _classname);
    }
public:
    virtual bool _try_lock(Ref<RxLockBase> self) {
        throw NotImplementedException();
    }
    virtual void _lock(Ref<RxLockBase> self) {
        throw NotImplementedException();
    }
    virtual void _unlock(Ref<RxLockBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateLock";
    }
};

class RxStartableTemplate_ : public RefCounted {
    GDCLASS(RxStartableTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxStartableTemplate_, _start);
        BIND_VIRTUAL_METHOD(RxStartableTemplate_, _await);
        BIND_VIRTUAL_METHOD(RxStartableTemplate_, _classname);
    }
public:
    virtual void _start(Ref<RxStartableBase> self) {
        throw NotImplementedException();
    }
    virtual void _await(Ref<RxStartableBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateStartable";
    }
};

class RxObservableTemplate_ : public RefCounted {
    GDCLASS(RxObservableTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxObservableTemplate_, _subscribe_core);
        BIND_VIRTUAL_METHOD(RxObservableTemplate_, _classname);
    }
public:
    virtual Ref<RxDisposableBase> _subscribe_core(Ref<RxObservable> self, Ref<RxObserverBase> observer, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateObservable";
    }
};

class RxObservableTemplate_Base_ : public RefCounted {
    GDCLASS(RxObservableTemplate_Base_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxObservableTemplate_Base_, _subscribe_observer);
        BIND_VIRTUAL_METHOD(RxObservableTemplate_Base_, _subscribe);
        BIND_VIRTUAL_METHOD(RxObservableTemplate_Base_, _classname);
    }
public:
    virtual Ref<RxDisposableBase> _subscribe_observer(Ref<RxObservable> self, Ref<RxObserverBase> observer, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _subscribe(Ref<RxObservable> self, Ref<RxOnNext> on_next, Ref<RxOnError> on_error, Ref<RxOnCompleted> on_completed, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateObservable_Base";
    }
};

class RxObserverTemplate_ : public RefCounted {
    GDCLASS(RxObserverTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxObserverTemplate_, _on_next);
        BIND_VIRTUAL_METHOD(RxObserverTemplate_, _on_error);
        BIND_VIRTUAL_METHOD(RxObserverTemplate_, _on_completed);
        BIND_VIRTUAL_METHOD(RxObserverTemplate_, _classname);
    }
public:
    virtual void _on_next(Ref<RxObserverBase> self, const Variant& value) {
        throw NotImplementedException();
    }
    virtual void _on_error(Ref<RxObserverBase> self, Ref<RxError> error) {
        throw NotImplementedException();
    }
    virtual void _on_completed(Ref<RxObserverBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateObserver";
    }
};

class RxSubjectTemplate_ : public RefCounted {
    GDCLASS(RxSubjectTemplate_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_, _subscribe_core);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_, _on_next_core);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_, _on_error_core);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_, _on_completed_core);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_, _classname);
    }
public:
    virtual Ref<RxDisposableBase> _subscribe_core(Ref<RxSubject> self, Ref<RxObserverBase> observer, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual void _on_next_core(Ref<RxSubject> self, const Variant& value) {
        throw NotImplementedException();
    }
    virtual void _on_error_core(Ref<RxSubject> self, Ref<RxError> error) {
        throw NotImplementedException();
    }
    virtual void _on_completed_core(Ref<RxSubject> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateSubject";
    }
};

class RxSubjectTemplate_Base_ : public RefCounted {
    GDCLASS(RxSubjectTemplate_Base_, RefCounted)

protected:
    inline static void _bind_methods() {
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _subscribe_observer);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _subscribe);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _on_next);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _on_error);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _on_completed);
        BIND_VIRTUAL_METHOD(RxSubjectTemplate_Base_, _classname);
    }
public:
    virtual Ref<RxDisposableBase> _subscribe_observer(Ref<RxSubjectBase> self, Ref<RxObserverBase> observer, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual Ref<RxDisposableBase> _subscribe(Ref<RxSubjectBase> self, Ref<RxOnNext> on_next, Ref<RxOnError> on_error, Ref<RxOnCompleted> on_completed, Ref<RxSchedulerBase> scheduler) {
        throw NotImplementedException();
    }
    virtual void _on_next(Ref<RxSubjectBase> self, const Variant& value) {
        throw NotImplementedException();
    }
    virtual void _on_error(Ref<RxSubjectBase> self, Ref<RxError> error) {
        throw NotImplementedException();
    }
    virtual void _on_completed(Ref<RxSubjectBase> self) {
        throw NotImplementedException();
    }
    virtual StringName _classname() const {
        return "TemplateSubject_Base";
    }
};

} // END namespace wrappers
} // END namespace rx

#endif // RX_WRAPPER_TEMPLATES_H