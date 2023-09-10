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

#define RX_BASEPTR_DECLARE(type, ptr_type, wrapper) \
    struct ptr_type : public std::shared_ptr<type> { \
        using std::shared_ptr<type>::shared_ptr; \
        ptr_type(const std::shared_ptr<type>& other) noexcept; \
        ptr_type(godot::Ref<wrapper> other); \
        ptr_type(const godot::Variant&); \
        operator godot::Ref<wrapper>(); \
        operator godot::Variant(); \
    };
RX_BASEPTR_DECLARE(rx::abstract::DisposableBase, disposable_t, rx::wrappers::RxDisposableBase)
RX_BASEPTR_DECLARE(rx::abstract::IterableBase, iterable_t, rx::wrappers::RxIterableBase)
RX_BASEPTR_DECLARE(rx::abstract::IteratorBase, iterator_t, rx::wrappers::RxIteratorBase)
RX_BASEPTR_DECLARE(rx::abstract::LockBase, lock_t, rx::wrappers::RxLockBase)
RX_BASEPTR_DECLARE(rx::abstract::ObservableBase, observable_t, rx::wrappers::RxObservableBase)
RX_BASEPTR_DECLARE(rx::abstract::ObserverBase, observer_t, rx::wrappers::RxObserverBase)
RX_BASEPTR_DECLARE(rx::abstract::PeriodicSchedulerBase, periodic_scheduler_t, rx::wrappers::RxPeriodicSchedulerBase)
RX_BASEPTR_DECLARE(rx::abstract::SchedulerBase, scheduler_t, rx::wrappers::RxSchedulerBase)
RX_BASEPTR_DECLARE(rx::abstract::StartableBase, startable_t, rx::wrappers::RxStartableBase)
RX_BASEPTR_DECLARE(rx::abstract::SubjectBase, subject_t, rx::wrappers::RxSubjectBase)

RX_BASEPTR_DECLARE(Notification, notification_t, rx::wrappers::RxNotification)
RX_BASEPTR_DECLARE(NotificationOnNext, notification_on_next_t, rx::wrappers::RxNotificationOnNext)
RX_BASEPTR_DECLARE(NotificationOnError, notification_on_error_t, rx::wrappers::RxNotificationOnError)
RX_BASEPTR_DECLARE(NotificationOnCompleted, notification_on_completed_t, rx::wrappers::RxNotificationOnCompleted)

RX_BASEPTR_DECLARE(rx::observable::Observable, rx_observable_t, rx::wrappers::RxObservable)
RX_BASEPTR_DECLARE(rx::subject::Subject, rx_subject_t, rx::wrappers::RxSubject)

/* Containers */

typedef std::list<rx_observable_t> observable_list_t;
typedef std::vector<rx_observable_t> observable_vec_t;
typedef std::list<disposable_t> disposable_list_t;
typedef std::vector<disposable_t> disposable_vec_t;

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

typedef std::function<rx_observable_t(const scheduler_t&)> observable_factory_t;
typedef std::function<rx_observable_t(const rx_observable_t&)> observable_op_t;

typedef std::function<void(const notification_t&)> notifier_t;

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