#ifndef RX_NOTIFICATION_H
#define RX_NOTIFICATION_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>

#include "abstract/observer.h"
#include "abstract/observable.h"
#include "internal/lambda.h"
#include "exception/exceptionwrapper.h"

using namespace godot;

class RxNotification : public RefCounted {
    GDCLASS(RxNotification, RefCounted)

public:
    bool has_value;
    Variant value;
    String kind;

protected:
    static void _bind_methods();

public:
    RxNotification() : has_value(false), kind("") {}
    ~RxNotification(){}

    void accept(const Variant& on_next, const Callable& on_error = DEFAULT_ON_ERROR, const Callable& on_completed = DEFAULT_ON_COMPLETED);

private:
    virtual void _accept(const Callable& on_next, const Callable& on_error = DEFAULT_ON_ERROR, const Callable& on_completed = DEFAULT_ON_COMPLETED);
    virtual void _accept_observer(Ref<ObserverBase> observer);
    Ref<ObservableBase> to_observable(Ref<SchedulerBase> scheduler = Ref<SchedulerBase>());

public:
    struct compare {
        bool operator()(Ref<RxNotification> lhs, Ref<RxNotification> rhs) const {
            auto left = lhs.is_null() ? "" : lhs->kind;
            auto right = rhs.is_null() ? " " : rhs->kind;
            return left == right;
        }
    };

};

class OnNext : public RxNotification {
    GDCLASS(OnNext, RxNotification)

public:
    OnNext(const Variant& value = Variant()) {
        this->has_value = true;
        this->value = value;
        this->kind = "N";
    }
    ~OnNext() {}

protected:
    static void _bind_methods();

public:
    void _accept(const Callable& on_next, const Callable& on_error = DEFAULT_ON_ERROR, const Callable& on_completed = DEFAULT_ON_COMPLETED) override;
    void _accept_observer(Ref<ObserverBase> observer) override;
    String _to_string() const;
};

class OnError : public RxNotification {
    GDCLASS(OnError, RxNotification)

private:
    Ref<RxError> exception;

public:
    OnError(Ref<RxError> error) : exception(error) {}
    OnError(const std::string& error) : exception(RxError::wrap(Exception(error))) {}
    OnError() : exception(RxError::wrap(Exception())) {}
    ~OnError() {}

protected:
    static void _bind_methods();

public:
    void _accept(const Callable& on_next, const Callable& on_error = DEFAULT_ON_ERROR, const Callable& on_completed = DEFAULT_ON_COMPLETED) override;
    void _accept_observer(Ref<ObserverBase> observer) override;
    String _to_string() const;

};

class OnCompleted : public RxNotification {
    GDCLASS(OnCompleted, RxNotification)

public:
    OnCompleted() {
        this->kind = "C";
    }
    ~OnCompleted() {}

protected:
    static void _bind_methods();

public:
    void _accept(const Callable& on_next, const Callable& on_error = DEFAULT_ON_ERROR, const Callable& on_completed = DEFAULT_ON_COMPLETED) override;
    void _accept_observer(Ref<ObserverBase> observer) override;
    String _to_string() const;
};

static Ref<ObserverBase> from_notifier(const Callable& handler);

#endif // RX_NOTIFICATION_H