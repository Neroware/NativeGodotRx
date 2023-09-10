#include "typing.h"

#include "abstract/disposable.h"

#include "wrapper/abstract.h"
#include "wrapper/observable.h"
#include "wrapper/subject.h"
#include "wrapper/notificationwrapper.h"

using namespace rx::abstract;
using namespace rx::wrappers;
using namespace godot;

namespace rx {

#define RX_BASEPTR_IMPL(ptr_type, base_type, Wrapper) \
    ptr_type::ptr_type(const std::shared_ptr<base_type>& other) noexcept \
        : std::shared_ptr<base_type>(other) {} \
    ptr_type::ptr_type(Ref<Wrapper> other) \
        : std::shared_ptr<base_type>(Wrapper::unwrap(other)) {} \
    ptr_type::ptr_type(const Variant& other) \
        : std::shared_ptr<base_type>(Wrapper::unwrap(other)) {} \
    ptr_type::operator Ref<Wrapper>() { \
        return Wrapper::wrap(*this); \
    } \
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

RX_BASEPTR_IMPL(rx_observable_t, Observable, RxObservable)
RX_BASEPTR_IMPL(rx_subject_t, Subject, RxSubject)

// #define RX_WRAPPERREF_IMPL

} // END namespace rx