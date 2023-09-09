#include "typing.h"

#include "abstract/disposable.h"

#include "wrapper/abstract.h"
#include "wrapper/notificationwrapper.h"

using namespace rx::abstract;
using namespace rx::wrappers;
using namespace godot;

namespace rx {

#define RX_BASEPTR_IMPL(ptr_type, base_type, Wrapper) \
    ptr_type::ptr_type(const std::shared_ptr<base_type>& other) noexcept \
        : std::shared_ptr<base_type>(other) {} \
    ptr_type::ptr_type(const Variant& other) \
        : std::shared_ptr<base_type>(Wrapper::unwrap(other)) {} \
    ptr_type::operator Variant() { \
        return Wrapper::wrap(*this); \
    }

RX_BASEPTR_IMPL(disposable_t, DisposableBase, RxDisposableBase)
RX_BASEPTR_IMPL(iterable_t, IterableBase, RxIterableBase)
RX_BASEPTR_IMPL(iterator_t, IteratorBase, RxIteratorBase)
RX_BASEPTR_IMPL(lock_t, LockBase, RxLockBase)
RX_BASEPTR_IMPL(observable_t, ObservableBase, RxObservableBase)
RX_BASEPTR_IMPL(observer_t, ObserverBase, RxObserverBase)
RX_BASEPTR_IMPL(periodic_scheduler_t, PeriodicSchedulerBase, RxPeriodicSchedulerBase)
RX_BASEPTR_IMPL(scheduler_t, SchedulerBase, RxSchedulerBase)
RX_BASEPTR_IMPL(startable_t, StartableBase, RxStartableBase)
RX_BASEPTR_IMPL(subject_t, SubjectBase, RxSubjectBase)

RX_BASEPTR_IMPL(notification_t, Notification, RxNotification)
RX_BASEPTR_IMPL(notification_on_next_t, NotificationOnNext, RxNotificationOnNext)
RX_BASEPTR_IMPL(notification_on_error_t, NotificationOnError, RxNotificationOnError)
RX_BASEPTR_IMPL(notification_on_completed_t, NotificationOnCompleted, RxNotificationOnCompleted)

// #define RX_WRAPPERREF_IMPL

#define RX_WRAPPERREF_IMPL(ref_type, ptr_type, Wrapper) \
    ref_type::ref_type(Ref<Wrapper> other) \
        : Ref<Wrapper>(other) {} \
    ref_type::ref_type(const ptr_type& other) \
        : Ref<Wrapper>(Wrapper::wrap(other)) {} \
    ref_type::operator ptr_type() { \
        return Wrapper::unwrap(*this); \
    }

RX_WRAPPERREF_IMPL(disposable_ref_t, disposable_t, RxDisposableBase)
RX_WRAPPERREF_IMPL(iterable_ref_t, iterable_t, RxIterableBase)
RX_WRAPPERREF_IMPL(iterator_ref_t, iterator_t, RxIteratorBase)
RX_WRAPPERREF_IMPL(lock_ref_t, lock_t, RxLockBase)
RX_WRAPPERREF_IMPL(observable_ref_t, observable_t, RxObservableBase)
RX_WRAPPERREF_IMPL(observer_ref_t, observer_t, RxObserverBase)
RX_WRAPPERREF_IMPL(periodic_scheduler_ref_t, periodic_scheduler_t, RxPeriodicSchedulerBase)
RX_WRAPPERREF_IMPL(scheduler_ref_t, scheduler_t, RxSchedulerBase)
RX_WRAPPERREF_IMPL(startable_ref_t, startable_t, RxStartableBase)
RX_WRAPPERREF_IMPL(subject_ref_t, subject_t, RxSubjectBase)

RX_WRAPPERREF_IMPL(notification_ref_t, notification_t, RxNotification)
RX_WRAPPERREF_IMPL(notification_on_next_ref_t, notification_on_next_t, RxNotificationOnNext)
RX_WRAPPERREF_IMPL(notification_on_error_ref_t, notification_on_error_t, RxNotificationOnError)
RX_WRAPPERREF_IMPL(notification_on_completed_ref_t, notification_on_completed_t, RxNotificationOnCompleted)

} // END namespace rx