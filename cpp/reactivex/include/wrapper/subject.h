#ifndef RX_WRAPPER_SUBJECT_H
#define RX_WRAPPER_SUBJECT_H

#include "wrapper/abstract.h"

#include "internal/utils.h"

#include "subject/subject.h"
#include "subject/behaviorsubject.h"
#include "subject/asyncsubject.h"
#include "subject/replaysubject.h"

using namespace rx::subject;

namespace rx {
namespace wrappers {

class RxSubject : public RxSubjectBase {
    GDCLASS(RxSubject, RxSubjectBase)
    RX_WRAPPER(RxSubject, Subject, RxSubjectBase, SubjectBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxSubject", D_METHOD("get"), &RxSubject::get);

        ClassDB::bind_method(D_METHOD("check_disposed"), &RxSubject::check_disposed);
        ClassDB::bind_method(D_METHOD("dispose"), &RxSubject::dispose);

        ClassDB::bind_method(D_METHOD("lock"), &RxSubject::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxSubject::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxSubject::try_lock);
        ClassDB::bind_method(D_METHOD("_set_is_disposed", "v"), &RxSubject::_set_is_disposed);
        ClassDB::bind_method(D_METHOD("_get_is_disposed"), &RxSubject::_get_is_disposed);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "_set_is_disposed", "_get_is_disposed");

        BIND_SETGET_PROPERTY(RxSubject, error, _set_error, _get_error, OBJECT)
        RX_WRAPPER_CAST_BINDS(RxSubject)
    }

public:
    inline static Ref<RxSubject> get() {
        return RxSubject::wrap(Subject::get());
    }

    inline void lock() { this->_ptr->lock.lock(); }
    inline void unlock() { this->_ptr->lock.unlock(); }
    inline bool try_lock() { return this->_ptr->lock.try_lock(); }
    inline void _set_is_disposed(bool v) { this->_ptr->is_disposed = v; }
    inline bool _get_is_disposed() { return this->_ptr->is_disposed; }

    inline void check_disposed() { this->_ptr->check_disposed(); }
    inline void dispose() { this->_ptr->dispose(); }

    inline Ref<RxError> _get_error() { return RxError::wrap(this->_ptr->exception); }
    inline void _set_error(Ref<RxError> error) { this->_ptr->exception = RxError::unwrap(error); }

}; // END class RxSubject

class RxReplaySubject : public RxSubject {
    GDCLASS(RxReplaySubject, RxSubject)
    RX_WRAPPER(RxReplaySubject, ReplaySubject, RxSubject, Subject)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxReplaySubject", D_METHOD("get", "buffer_size", "window", "scheduler"), &RxReplaySubject::get, DEFVAL(1024UL), DEFVAL(memnew(RelativeTime(std::chrono::milliseconds::max()))), DEFVAL(RxSchedulerBase::wrap(CurrentThreadScheduler::get())));
        
        BIND_SETGET_PROPERTY(RxReplaySubject, buffer_size, _set_buffer_size, _get_buffer_size, INT)
        BIND_SETGET_PROPERTY(RxReplaySubject, scheduler, _set_scheduler, _get_scheduler, OBJECT)
        BIND_SETGET_PROPERTY(RxReplaySubject, window, _set_window, _get_window, OBJECT)

        RX_WRAPPER_CAST_BINDS(RxReplaySubject)
    }

public:
    inline static Ref<RxReplaySubject> get(
        size_t buffer_size = 1024UL, 
        Ref<RelativeTime> window = memnew(RelativeTime(std::chrono::milliseconds::max())), 
        Ref<RxSchedulerBase> scheduler = RxSchedulerBase::wrap(CurrentThreadScheduler::get())
    ) {
        return RxReplaySubject::wrap(ReplaySubject::get(
            buffer_size, window->dt, RxSchedulerBase::unwrap(scheduler)));
    }

    inline void _set_buffer_size(size_t v) { this->_ptr->buffer_size = v; }
    inline size_t _get_buffer_size() { return this->_ptr->buffer_size; }

    inline void _set_scheduler(Ref<RxSchedulerBase> scheduler) { this->_ptr->scheduler = RxSchedulerBase::unwrap(scheduler); }
    inline Ref<RxSchedulerBase> _get_scheduler() { return RxSchedulerBase::wrap(this->_ptr->scheduler); }

    inline void _set_window(Ref<RelativeTime> v) { this->_ptr->window = v->dt; }
    inline Ref<RelativeTime> _get_window() { return memnew(RelativeTime(this->_ptr->window)); }


}; // END class RxReplaySubject

class RxBehaviorSubject : public RxSubject {
    GDCLASS(RxBehaviorSubject, RxSubject)
    RX_WRAPPER(RxBehaviorSubject, BehaviorSubject, RxSubject, Subject)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxBehaviorSubject", D_METHOD("get", "value"), &RxBehaviorSubject::get);

        BIND_SETGET_PROPERTY(RxBehaviorSubject, value, _set_value, _get_value, VARIANT_MAX)
        RX_WRAPPER_CAST_BINDS(RxBehaviorSubject)
    }

public:
    inline static Ref<RxBehaviorSubject> get(const Variant& value) { 
        return RxBehaviorSubject::wrap(BehaviorSubject::get(value));
    }

    inline void _set_value(const Variant& v) { this->_ptr->value = v; }
    inline Variant _get_value() { return this->_ptr->value; } 


}; // END class RxBehaviorSubject

class RxAsyncSubject : public RxSubject {
    GDCLASS(RxAsyncSubject, RxSubject)
    RX_WRAPPER(RxAsyncSubject, AsyncSubject, RxSubject, Subject)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxAsyncSubject", D_METHOD("get"), &RxAsyncSubject::get);

        BIND_SETGET_PROPERTY(RxAsyncSubject, value, _set_value, _get_value, VARIANT_MAX)
        BIND_SETGET_PROPERTY(RxAsyncSubject, has_value, _set_has_value, _get_has_value, BOOL)
        RX_WRAPPER_CAST_BINDS(RxAsyncSubject)
    }

public:
    inline static Ref<RxAsyncSubject> get() { 
        return RxAsyncSubject::wrap(AsyncSubject::get());
    }

    inline void _set_value(const Variant& v) { this->_ptr->value = v; }
    inline Variant _get_value() { return this->_ptr->value; }

    inline void _set_has_value(bool v) { this->_ptr->has_value = v; }
    inline bool _get_has_value() { return this->_ptr->has_value; }


}; // END class RxAsyncSubject

} // END namespace wrappers
} // END namespace rx

#endif // RX_WRAPPER_SUBJECT_H