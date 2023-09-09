#ifndef RX_TYPING_H
#define RX_TYPING_H

#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/ref.hpp>

#include <functional>
#include <exception>
#include <memory>


namespace rx {

/* Exception */

typedef std::exception_ptr error_t;

/* GodotRx Base Types */

namespace abstract {
    class DisposableBase;
    class IterableBase;
    class IteratorBase;
    class LockBase;
    class ObservableBase;
    class ObserverBase;
    class PeriodicSchedulerBase;
    class SchedulerBase;
    class StartableBase;
    class SubjectBase;
}
namespace observable {
    class Observable;
}
namespace subject {
    class Subject;
}
class Notification;
class NotificationOnNext;
class NotificationOnError;
class NotificationOnCompleted;

#define RX_BASEPTR_DECLARE(type, ptr_type) \
    struct ptr_type : public std::shared_ptr<type> { \
        using std::shared_ptr<type>::shared_ptr; \
        ptr_type(const std::shared_ptr<type>& other) noexcept; \
        ptr_type(const godot::Variant&); \
        operator godot::Variant(); \
    };
RX_BASEPTR_DECLARE(rx::abstract::DisposableBase, disposable_t)
RX_BASEPTR_DECLARE(rx::abstract::IterableBase, iterable_t)
RX_BASEPTR_DECLARE(rx::abstract::IteratorBase, iterator_t)
RX_BASEPTR_DECLARE(rx::abstract::LockBase, lock_t)
RX_BASEPTR_DECLARE(rx::abstract::ObservableBase, observable_t)
RX_BASEPTR_DECLARE(rx::abstract::ObserverBase, observer_t)
RX_BASEPTR_DECLARE(rx::abstract::PeriodicSchedulerBase, periodic_scheduler_t)
RX_BASEPTR_DECLARE(rx::abstract::SchedulerBase, scheduler_t)
RX_BASEPTR_DECLARE(rx::abstract::StartableBase, startable_t)
RX_BASEPTR_DECLARE(rx::abstract::SubjectBase, subject_t)

RX_BASEPTR_DECLARE(Notification, notification_t)
RX_BASEPTR_DECLARE(NotificationOnNext, notification_on_next_t)
RX_BASEPTR_DECLARE(NotificationOnError, notification_on_error_t)
RX_BASEPTR_DECLARE(NotificationOnCompleted, notification_on_completed_t)

RX_BASEPTR_DECLARE(rx::observable::Observable, rx_observable_t)
RX_BASEPTR_DECLARE(rx::subject::Subject, rx_subject_t)

/* GodotRx Wrapper Types */

namespace wrappers {
    class RxDisposableBase;
    class RxIterableBase;
    class RxIteratorBase;
    class RxLockBase;
    class RxObservableBase;
    class RxObserverBase;
    class RxPeriodicSchedulerBase;
    class RxSchedulerBase;
    class RxStartableBase;
    class RxSubjectBase;

    class RxNotification;
    class RxNotificationOnNext;
    class RxNotificationOnError;
    class RxNotificationOnCompleted;

    class RxSubject;
    class RxObservable;
};

#define RX_WRAPPERREF_DECLARE(type, ref_type, ptr_type) \
    struct ref_type : public godot::Ref<type> { \
        using godot::Ref<type>::Ref; \
        ref_type(Ref<type> other); \
        ref_type(const ptr_type& other); \
        operator ptr_type(); \
    };
RX_WRAPPERREF_DECLARE(rx::wrappers::RxDisposableBase, disposable_ref_t, disposable_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxIterableBase, iterable_ref_t, iterable_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxIteratorBase, iterator_ref_t, iterator_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxLockBase, lock_ref_t, lock_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxObservableBase, observable_ref_t, observable_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxObserverBase, observer_ref_t, observer_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxPeriodicSchedulerBase, periodic_scheduler_ref_t, periodic_scheduler_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxSchedulerBase, scheduler_ref_t, scheduler_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxStartableBase, startable_ref_t, startable_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxSubjectBase, subject_ref_t, subject_t)

RX_WRAPPERREF_DECLARE(rx::wrappers::RxNotification, notification_ref_t, notification_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxNotificationOnNext, notification_on_next_ref_t, notification_on_next_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxNotificationOnError, notification_on_error_ref_t, notification_on_error_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxNotificationOnCompleted, notification_on_completed_ref_t, notification_on_completed_t)

RX_WRAPPERREF_DECLARE(rx::wrappers::RxSubject, rx_subject_ref_t, rx_subject_t)
RX_WRAPPERREF_DECLARE(rx::wrappers::RxObservable, rx_observable_ref_t, rx_observable_t)

/* Containers */

typedef std::list<rx_observable_t> observable_list_t;
typedef std::vector<rx_observable_t> observable_vec_t;

/* Functionals */

typedef std::function<void()> dispose_t;

typedef std::function<void(const godot::Variant&)> on_next_t;
typedef std::function<void(const error_t&)> on_error_t;
typedef std::function<void()> on_completed_t;

typedef std::function<disposable_t(const observer_t&, const scheduler_t&)> subscription_t;

typedef std::function<godot::Variant(const godot::Variant&)> periodic_action_t;
typedef std::function<disposable_t(const scheduler_t&, const godot::Variant&)> scheduled_action_t;

typedef std::function<godot::Variant()> run_t;
typedef std::function<startable_t(const run_t&)> thread_factory_t;

typedef std::function<bool(const error_t& e)> handler_t;
typedef std::function<void(const notification_t&)> notification_handler_t;

template<typename KeyT, typename... ArgsT>
using mapper_t = std::function<KeyT(const ArgsT&...)>;

template<typename KeyT, typename T>
using mapper_indexed_t = mapper_t<KeyT, T, uint64_t>;

template<typename... ArgsT>
using predicate_t = std::function<bool(const ArgsT&...)>;

template<typename T>
using predicate_indexed_t = predicate_t<T, uint64_t>;

template<typename T>
using comparer_t = std::function<bool(const T&, const T&)>;

template<typename T>
using sub_comparer_t = std::function<int(const T&, const T&)>;

template<typename StateT, typename T>
using accumulator_t = std::function<StateT(const StateT&, const T&)>;

} // END namespace rx

#endif // RX_TYPING_H