#ifndef RX_WRAPPER_NOTIFICATION_H
#define RX_WRAPPER_NOTIFICATION_H

#include "notification.h"
#include "wrapper/abstract.h"

#include "wrapper/wrapper.h"
#include "internal/utils.h"

namespace rx::wrappers {

#define NOTIFICATION_NEXT(value) RxNotificationOnNext::wrap(NotificationOnNext::get(value))
#define NOTIFICATION_ERROR(error) RxNotificationOnError::wrap(NotificationOnError::get(error))
#define NOTIFICATION_COMPLETED() RxNotificationOnCompleted::wrap(NotificationOnCompleted::get())

class RxNotification : public RefCounted {
    GDCLASS(RxNotification, RefCounted)
    _RX_ABSTRACT_WRAPPER(RxNotification, Notification)
    RX_WRAPPER_EQUALITY(RxNotification)

    inline String _to_string() const {
        return this->_ptr->to_string();
    }

    inline bool _get_has_value() {
        return this->_ptr->has_value;
    }
    inline Variant _get_value() {
        return this->_ptr->value;
    }
    inline String _get_kind() {
        return this->_ptr->kind;
    }

    inline void accept(const Callable& on_next, const Callable& on_error = Callable(), const Callable& on_completed = Callable()) {
        this->_ptr->accept(
            on_next_cb(on_next),
            on_error.is_null() ? DEFAULT_ON_ERROR : on_error_cb(on_error),
            on_completed.is_null() ? DEFAULT_ON_COMPLETED : on_completed_cb(on_completed)
        );
    }
    inline void accept_observer(Ref<RxObserverBase> observer) {
        this->_ptr->accept(RxObserverBase::unwrap(observer));
    }

    inline Ref<RxObservableBase> to_observable(Ref<RxSchedulerBase> scheduler = VNULL) {
        return RxObservableBase::wrap(this->_ptr->to_observable(RxSchedulerBase::unwrap(scheduler)));
    }

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxNotification::equals);
        ClassDB::bind_method(D_METHOD("to_observable", "scheduler"), &RxNotification::to_observable, DEFVAL(Ref<RxSchedulerBase>()));
        ClassDB::bind_method(D_METHOD("accept", "on_next", "on_error", "on_completed"), &RxNotification::accept, DEFVAL(Callable()), DEFVAL(Callable()));
        ClassDB::bind_method(D_METHOD("accept_observer", "observer"), &RxNotification::accept_observer);
        BIND_GET_PROPERTY(RxNotification, has_value, _get_has_value, BOOL);
        BIND_GET_PROPERTY(RxNotification, value, _get_value, VARIANT_MAX);
        BIND_GET_PROPERTY(RxNotification, kind, _get_kind, STRING);
        RX_WRAPPER_CAST_BINDS(RxNotification)
    }

};

class RxNotificationOnNext : public RxNotification {
    GDCLASS(RxNotificationOnNext, RxNotification)
    _RX_WRAPPER(RxNotificationOnNext, NotificationOnNext, RxNotification, Notification)

    inline String _to_string() const {
        return this->_ptr->to_string();
    }

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnNext", D_METHOD("get", "value"), &RxNotificationOnNext::get, DEFVAL(VNULL));
        RX_WRAPPER_CAST_BINDS(RxNotificationOnNext)
    }
public:
    inline static Ref<RxNotificationOnNext> get(const Variant& value = Variant()) {
        return RxNotificationOnNext::wrap(NotificationOnNext::get(value));
    }

}; // END class RxNotificationOnNext

class RxNotificationOnError : public RxNotification {
    GDCLASS(RxNotificationOnError, RxNotification)
    _RX_WRAPPER(RxNotificationOnError, NotificationOnError, RxNotification, Notification)

    inline String _to_string() const {
        return this->_ptr->to_string();
    }

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnError", D_METHOD("get", "value"), &RxNotificationOnError::get);
        BIND_GET_PROPERTY(RxNotificationOnError, error, _get_error, OBJECT)
        RX_WRAPPER_CAST_BINDS(RxNotificationOnError)
    }
public:
    inline static Ref<RxNotificationOnError> get(Ref<RxError> error) {
        return RxNotificationOnError::wrap(NotificationOnError::get(RxError::unwrap(error)));
    }

    inline Ref<RxError> _get_error() {
        return RxError::wrap(this->_ptr->exception);
    }

}; // END class RxNotificationOnCompleted

class RxNotificationOnCompleted : public RxNotification {
    GDCLASS(RxNotificationOnCompleted, RxNotification)
    _RX_WRAPPER(RxNotificationOnCompleted, NotificationOnCompleted, RxNotification, Notification)

    inline String _to_string() const {
        return this->_ptr->to_string();
    }

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnCompleted", D_METHOD("get"), &RxNotificationOnCompleted::get);
        RX_WRAPPER_CAST_BINDS(RxNotificationOnCompleted)
    }
public:
    inline static Ref<RxNotificationOnCompleted> get() {
        return RxNotificationOnCompleted::wrap(NotificationOnCompleted::get());
    }

}; // END class RxNotificationOnCompleted

} // END namespace rx::wrapper

#endif // RX_WRAPPER_NOTIFICATION_H

