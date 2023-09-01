#ifndef RX_WRAPPER_NOTIFICATION_H
#define RX_WRAPPER_NOTIFICATION_H

#include "wrapper/abstract.h"

namespace rx::wrappers {

class RxNotificationOnNext : public RxNotification {
    GDCLASS(RxNotificationOnNext, RxNotification)
    RX_WRAPPER(RxNotificationOnNext, notification_on_next_t, RxNotification, notification_t)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnNext", D_METHOD("get", "value"), &RxNotificationOnNext::get, DEFVAL(VNULL));
        RX_WRAPPER_CAST_BINDS(RxNotificationOnNext)
    }
public:
    inline static Ref<RxNotificationOnNext> get(const Variant& value = Variant()) {
        return RxNotificationOnNext::wrap(notification_on_next_t::get(value));
    }

}; // END class RxNotificationOnNext

class RxNotificationOnError : public RxNotification {
    GDCLASS(RxNotificationOnError, RxNotification)
    RX_WRAPPER(RxNotificationOnError, notification_on_error_t, RxNotification, notification_t)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnError", D_METHOD("get", "value"), &RxNotificationOnError::get);
        BIND_GET_PROPERTY(RxNotificationOnError, error, _get_error, OBJECT)
        RX_WRAPPER_CAST_BINDS(RxNotificationOnError)
    }
public:
    inline static Ref<RxNotificationOnError> get(Ref<RxError> error) {
        return RxNotificationOnError::wrap(notification_on_error_t::get(RxError::unwrap(error)));
    }

    inline Ref<RxError> _get_error() {
        return RxError::wrap(this->_ptr->exception);
    }

}; // END class RxNotificationOnCompleted

class RxNotificationOnCompleted : public RxNotification {
    GDCLASS(RxNotificationOnCompleted, RxNotification)
    RX_WRAPPER(RxNotificationOnCompleted, notification_on_completed_t, RxNotification, notification_t)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxNotificationOnCompleted", D_METHOD("get"), &RxNotificationOnCompleted::get);
        RX_WRAPPER_CAST_BINDS(RxNotificationOnCompleted)
    }
public:
    inline static Ref<RxNotificationOnCompleted> get() {
        return RxNotificationOnCompleted::wrap(notification_on_completed_t::get());
    }

}; // END class RxNotificationOnCompleted

} // END namespace rx::wrapper

#endif // RX_WRAPPER_NOTIFICATION_H

