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
    }
public:
    RxOnNext() : on_next(DEFAULT_ON_NEXT) {}
    RxOnNext(const on_next_t& on_next_) : on_next(on_next_) {}
    inline void invoke(const Variant& item) { this->on_next(item); }
};

class RxOnError : public RefCounted {
    GDCLASS(RxOnError, RefCounted)

private:
    on_error_t on_error;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "error"), &RxOnError::invoke);
    }
public:
    RxOnError() : on_error(DEFAULT_ON_ERROR) {}
    RxOnError(const on_error_t& on_error_) : on_error(on_error_) {}
    inline void invoke(Ref<RxError> error) { this->on_error(RxError::unwrap(error)); }
};

class RxOnCompleted : public RefCounted {
    GDCLASS(RxOnCompleted, RefCounted)

private:
    on_completed_t on_completed;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke"), &RxOnCompleted::invoke);
    }
public:
    RxOnCompleted() {}
    RxOnCompleted(const on_completed_t& on_completed_) : on_completed(on_completed_) {}
    inline void invoke() { this->on_completed(); }
};

class RxAction : public RefCounted {
    GDCLASS(RxAction, RefCounted)

private:
    scheduled_action_t action;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "scheduler", "state"), &RxAction::invoke, DEFVAL(VNULL));
    }
public:
    RxAction() {}
    RxAction(const scheduled_action_t& action_) : action(action_) {}
    inline Ref<RxDisposableBase> invoke(Ref<RxSchedulerBase> scheduler, const Variant& state = VNULL) { return this->action(scheduler, state); }
};

class RxPeriodicAction : public RefCounted {
    GDCLASS(RxPeriodicAction, RefCounted)

private:
    periodic_action_t action;
protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("invoke", "state"), &RxPeriodicAction::invoke, DEFVAL(VNULL));
    }
public:
    RxPeriodicAction() {}
    RxPeriodicAction(const periodic_action_t& action_) : action(action_) {}
    inline Variant invoke(const Variant& state = VNULL) { return this->action(state); }
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
    Ref<RxDisposableBase> _schedule_periodic(Ref<RxPeriodicSchedulerBase> self, Ref<RelativeTime> duetime, Ref<RxPeriodicAction> action, const Variant& state = Variant()) {
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
    Ref<RxDisposableBase> _schedule_signal(Ref<RxGodotSignalSchedulerBase> self, Object* owner, const StringName& signal, Ref<RxPeriodicAction> action, const Variant& state = Variant()) {
        throw NotImplementedException();
    }
};

} // END namespace wrappers
} // END namespace rx

#endif // RX_WRAPPER_TEMPLATES_H