#ifndef RX_WRAPPER_OBSERVER_H
#define RX_WRAPPER_OBSERVER_H

#include "wrapper/abstract.h"

#include "internal/utils.h"

#include "observer/observer.h"
#include "observer/autodetachobserver.h"
#include "observer/scheduledobserver.h"
#include "observer/observeonobserver.h"

#include "templates/observer.h"

using namespace rx::observer;
using namespace rx::templates;

namespace rx {
namespace wrappers {

class RxObserver : public RxObserverBase {
    GDCLASS(RxObserver, RxObserverBase)
    RX_WRAPPER(RxObserver, Observer, RxObserverBase, ObserverBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxObserver", D_METHOD("get", "on_next", "on_error", "on_completed"), &RxObserver::get);
        ClassDB::bind_method(D_METHOD("dispose"), &RxObserver::dispose);
        ClassDB::bind_method(D_METHOD("fail", "error"), &RxObserver::fail);
        ClassDB::bind_method(D_METHOD("throw", "error"), &RxObserver::throw_error);
        ClassDB::bind_method(D_METHOD("as_observer"), &RxObserver::as_observer);
        RX_WRAPPER_CAST_BINDS(RxObserver)
    }

public:
    inline static Ref<RxObserver> get(const Callable& on_next_, const Callable& on_error_, const Callable& on_completed_) {
        return RxObserver::wrap(Observer::get(
            on_next_cb(on_next_),
            on_error_cb(on_error_),
            on_completed_cb(on_completed_)
        ));
    }

    inline void dispose() { this->_ptr->dispose(); }
    inline bool fail(Ref<RxError> error) { return this->_ptr->fail(RxError::unwrap(error)); }
    inline bool throw_error(Ref<RxError> error) { this->_ptr->throw_error(RxError::unwrap(error)); }
    inline Ref<RxObserverBase> as_observer() { return RxObserverBase::wrap(this->_ptr->as_observer()); }

}; // END class RxObserver

class RxScheduledObserver : public RxObserver {
    GDCLASS(RxScheduledObserver, RxObserver)
    RX_WRAPPER(RxScheduledObserver, ScheduledObserver, RxObserver, Observer)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxScheduledObserver", D_METHOD("get", "observer", "scheduler"), &RxScheduledObserver::get);

        ClassDB::bind_method(D_METHOD("lock"), &RxScheduledObserver::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxScheduledObserver::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxScheduledObserver::try_lock);

        ClassDB::bind_method(D_METHOD("_set_is_acquired", "v"), &RxScheduledObserver::_set_is_acquired);
        ClassDB::bind_method(D_METHOD("_get_is_acquired"), &RxScheduledObserver::_get_is_acquired);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_acquired"), "_set_is_acquired", "_get_is_acquired");

        ClassDB::bind_method(D_METHOD("_set_has_faulted", "v"), &RxScheduledObserver::_set_has_faulted);
        ClassDB::bind_method(D_METHOD("_get_has_faulted"), &RxScheduledObserver::_get_has_faulted);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_faulted"), "_set_has_faulted", "_get_has_faulted");

        ClassDB::bind_method(D_METHOD("_set_scheduler", "v"), &RxScheduledObserver::_set_scheduler);
        ClassDB::bind_method(D_METHOD("_get_scheduler"), &RxScheduledObserver::_get_scheduler);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scheduler"), "_set_scheduler", "_get_scheduler");

        ClassDB::bind_method(D_METHOD("_set_observer", "v"), &RxScheduledObserver::_set_observer);
        ClassDB::bind_method(D_METHOD("_get_observer"), &RxScheduledObserver::_get_observer);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "observer"), "_set_observer", "_get_observer");

        ClassDB::bind_method(D_METHOD("ensure_active"), &RxScheduledObserver::ensure_active);
        ClassDB::bind_method(D_METHOD("run", "scheduler", "state"), &RxScheduledObserver::run);

        RX_WRAPPER_CAST_BINDS(RxScheduledObserver)
    }

public:
    inline static Ref<RxScheduledObserver> get(Ref<RxSchedulerBase> scheduler, Ref<RxObserverBase> observer) {
        return RxScheduledObserver::wrap(ScheduledObserver::get(
            RxSchedulerBase::unwrap(scheduler),
            RxObserverBase::unwrap(observer)
        ));
    }

    inline void lock() { this->_ptr->lock.lock(); }
    inline void unlock() { this->_ptr->lock.unlock(); }
    inline bool try_lock() { return this->_ptr->lock.try_lock(); }

    inline void _set_is_acquired(bool v) { this->_ptr->is_acquired = v; } \
    inline bool _get_is_acquired() { return this->_ptr->is_acquired; }

    inline void _set_has_faulted(bool v) { this->_ptr->has_faulted = v; } \
    inline bool _get_has_faulted() { return this->_ptr->has_faulted; }

    inline void _set_scheduler(Ref<RxSchedulerBase> scheduler) { this->_ptr->scheduler = RxSchedulerBase::unwrap(scheduler); } \
    inline Ref<RxSchedulerBase> _get_scheduler() { return RxSchedulerBase::wrap(this->_ptr->scheduler); }

    inline void _set_observer(Ref<RxObserverBase> observer) { this->_ptr->observer = RxObserverBase::unwrap(observer); } \
    inline Ref<RxObserverBase> _get_observer() { return RxObserverBase::wrap(this->_ptr->observer); }

    inline void ensure_active() { this->_ptr->ensure_active(); }
    inline void run(Ref<RxSchedulerBase> scheduler, const Variant& state) { this->_ptr->run(RxSchedulerBase::unwrap(scheduler), state); }

}; // END class RxScheduledObserver

class RxObserveOnObserver : public RxScheduledObserver {
    GDCLASS(RxObserveOnObserver, RxScheduledObserver)
    RX_WRAPPER(RxObserveOnObserver, ObserveOnObserver, RxScheduledObserver, ScheduledObserver)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxObserveOnObserver", D_METHOD("get", "observer", "scheduler"), &RxObserveOnObserver::get);
        RX_WRAPPER_CAST_BINDS(RxObserveOnObserver)
    }

public:
    inline static Ref<RxObserveOnObserver> get(Ref<RxSchedulerBase> scheduler, Ref<RxObserverBase> observer) {
        return RxObserveOnObserver::wrap(ObserveOnObserver::get(
            RxSchedulerBase::unwrap(scheduler),
            RxObserverBase::unwrap(observer)
        ));
    }

}; // END class RxObserveOnObserver

class RxAutoDetachObserver : public RxObserverBase {
    GDCLASS(RxAutoDetachObserver, RxObserverBase)
    RX_WRAPPER(RxAutoDetachObserver, AutoDetachObserver, RxObserverBase, ObserverBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxAutoDetachObserver", D_METHOD("get", "on_next", "on_error", "on_completed"), &RxAutoDetachObserver::get);

        BIND_SETGET_PROPERTY(RxAutoDetachObserver, is_stopped, _set_is_stopped, _get_is_stopped, BOOL)

        ClassDB::bind_method(D_METHOD("set_disposable", "disp"), &RxAutoDetachObserver::set_disposable);
        ClassDB::bind_method(D_METHOD("dispose"), &RxAutoDetachObserver::dispose);
        ClassDB::bind_method(D_METHOD("fail", "error"), &RxAutoDetachObserver::fail);

        RX_WRAPPER_CAST_BINDS(RxAutoDetachObserver)
    }

public:
    inline static Ref<RxAutoDetachObserver> get(const Callable& on_next_, const Callable& on_error_, const Callable& on_completed_) {
        return RxAutoDetachObserver::wrap(std::make_shared<AutoDetachObserver>(
            on_next_cb(on_next_),
            on_error_cb(on_error_),
            on_completed_cb(on_completed_)
        ));
    }

    inline void set_disposable(Ref<RxDisposableBase> disp) { this->_ptr->set_disposable(RxDisposableBase::unwrap(disp)); }
    inline void dispose() { this->_ptr->dispose(); }
    inline bool fail(Ref<RxError> error) { return this->_ptr->fail(RxError::unwrap(error)); }

    inline void _set_is_stopped(bool v) { this->_ptr->is_stopped = v; } \
    inline bool _get_is_stopped() { return this->_ptr->is_stopped; }

}; // END class RxObserveOnObserver


class RxObserver_ : public RxObserverBase {
    GDCLASS(RxObserver_, RxObserverBase)
    _RX_WRAPPER(RxObserver_, Observer_, RxObserverBase, ObserverBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxObserver_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxObserver_::_template);
    }

public:
    RxObserver_() 
        : RxObserverBase(std::static_pointer_cast<ObserverBase>(Observer_::get())), 
        _ptr(std::static_pointer_cast<Observer_>(RxObserverBase::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxObserverTemplate_> t) {
        this->_ptr->_template(t);
    }
    
}; // END class RxObserver_


} // END namespace wrappers
} // END namespace rx

#endif // RX_WRAPPER_OBSERVER_H