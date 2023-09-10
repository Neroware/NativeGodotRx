#ifndef RX_WRAPPER_DISPOSABLE_H
#define RX_WRAPPER_DISPOSABLE_H

#include "wrapper/abstract.h"

#include "disposable/booleandisposable.h"
#include "disposable/compositedisposable.h"
#include "disposable/disposable.h"
#include "disposable/multiassignmentdisposable.h"
#include "disposable/refcountdisposable.h"
#include "disposable/scheduleddisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/singleassignmentdisposable.h"

#include "internal/iterator.h"

using namespace rx::disposable;

namespace rx {
namespace wrappers {

#define DISPOSABLE_SETGET \
    inline void lock() { this->_ptr->lock.lock(); } \
    inline void unlock() { this->_ptr->lock.unlock(); } \
    inline bool try_lock() { return this->_ptr->lock.try_lock(); } \
    inline void _set_is_disposed(bool v) { this->_ptr->is_disposed = v; } \
    inline bool _get_is_disposed() { return this->_ptr->is_disposed; }

#define DISPOSABLE_SETGET_BIND(Class) \
    ClassDB::bind_method(D_METHOD("lock"), &Class::lock); \
    ClassDB::bind_method(D_METHOD("unlock"), &Class::unlock); \
    ClassDB::bind_method(D_METHOD("try_lock"), &Class::try_lock); \
    ClassDB::bind_method(D_METHOD("_set_is_disposed", "v"), &Class::_set_is_disposed); \
    ClassDB::bind_method(D_METHOD("_get_is_disposed"), &Class::_get_is_disposed); \
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "_set_is_disposed", "_get_is_disposed");

class RxBooleanDisposable : public RxDisposableBase {
    GDCLASS(RxBooleanDisposable, RxDisposableBase)
    RX_WRAPPER(RxBooleanDisposable, BooleanDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxBooleanDisposable)
        RX_WRAPPER_CAST_BINDS(RxBooleanDisposable)
        ClassDB::bind_static_method("RxBooleanDisposable", D_METHOD("get"), &RxBooleanDisposable::get);
    }
public:
    // constructor
    inline static Ref<RxBooleanDisposable> get() { return RxBooleanDisposable::wrap(std::make_shared<BooleanDisposable>()); }
    // set-get
    DISPOSABLE_SETGET

}; // END class RxBooleanDisposable

class RxSingleAssignmentDisposable : public RxDisposableBase {
    GDCLASS(RxSingleAssignmentDisposable, RxDisposableBase)
    RX_WRAPPER(RxSingleAssignmentDisposable, SingleAssignmentDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxSingleAssignmentDisposable)
        RX_WRAPPER_CAST_BINDS(RxSingleAssignmentDisposable)
        ClassDB::bind_static_method("RxSingleAssignmentDisposable", D_METHOD("get"), &RxSingleAssignmentDisposable::get);

        ClassDB::bind_method(D_METHOD("get_disposable"), &RxSingleAssignmentDisposable::get_disposable);
        ClassDB::bind_method(D_METHOD("set_disposable", "value"), &RxSingleAssignmentDisposable::set_disposable);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "set_disposable", "get_disposable");
    }
public:
    // constructor
    inline static Ref<RxSingleAssignmentDisposable> get() { return RxSingleAssignmentDisposable::wrap(std::make_shared<SingleAssignmentDisposable>()); }
    // set-get
    DISPOSABLE_SETGET
    inline Ref<RxDisposableBase> get_disposable() { return RxDisposableBase::wrap(this->_ptr->get_disposable()); }
    inline void set_disposable(Ref<RxDisposableBase> d) { return this->_ptr->set_disposable(RxDisposableBase::unwrap(d)); }

}; // END class RxSingleAssignmentDisposable

class RxMultiAssignmentDisposable : public RxDisposableBase {
    GDCLASS(RxMultiAssignmentDisposable, RxDisposableBase)
    RX_WRAPPER(RxMultiAssignmentDisposable, MultiAssignmentDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxMultiAssignmentDisposable)
        RX_WRAPPER_CAST_BINDS(RxMultiAssignmentDisposable)
        ClassDB::bind_static_method("RxMultiAssignmentDisposable", D_METHOD("get"), &RxMultiAssignmentDisposable::get);

        ClassDB::bind_method(D_METHOD("get_disposable"), &RxMultiAssignmentDisposable::get_disposable);
        ClassDB::bind_method(D_METHOD("set_disposable", "value"), &RxMultiAssignmentDisposable::set_disposable);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "set_disposable", "get_disposable");
    }
public:
    // constructor
    inline static Ref<RxMultiAssignmentDisposable> get() { return RxMultiAssignmentDisposable::wrap(std::make_shared<MultiAssignmentDisposable>()); }
    // set-get
    DISPOSABLE_SETGET
    inline Ref<RxDisposableBase> get_disposable() { return RxDisposableBase::wrap(this->_ptr->get_disposable()); }
    inline void set_disposable(Ref<RxDisposableBase> d) { return this->_ptr->set_disposable(RxDisposableBase::unwrap(d)); }

}; // END class RxMultiAssignmentDisposable

class RxSerialDisposable : public RxDisposableBase {
    GDCLASS(RxSerialDisposable, RxDisposableBase)
    RX_WRAPPER(RxSerialDisposable, SerialDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxSerialDisposable)
        RX_WRAPPER_CAST_BINDS(RxSerialDisposable)
        ClassDB::bind_static_method("RxSerialDisposable", D_METHOD("get"), &RxSerialDisposable::get);

        ClassDB::bind_method(D_METHOD("get_disposable"), &RxSerialDisposable::get_disposable);
        ClassDB::bind_method(D_METHOD("set_disposable", "value"), &RxSerialDisposable::set_disposable);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "set_disposable", "get_disposable");
    }
public:
    // constructor
    inline static Ref<RxSerialDisposable> get() { return RxSerialDisposable::wrap(std::make_shared<SerialDisposable>()); }
    // set-get
    DISPOSABLE_SETGET
    inline Ref<RxDisposableBase> get_disposable() { return RxDisposableBase::wrap(this->_ptr->get_disposable()); }
    inline void set_disposable(Ref<RxDisposableBase> d) { return this->_ptr->set_disposable(RxDisposableBase::unwrap(d)); }

}; // END class RxSerialDisposable

class RxDisposable : public RxDisposableBase {
    GDCLASS(RxDisposable, RxDisposableBase)
    RX_WRAPPER(RxDisposable, Disposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxDisposable)
        RX_WRAPPER_CAST_BINDS(RxDisposable)
        ClassDB::bind_static_method("RxDisposable", D_METHOD("get", "dispose"), &RxDisposable::get);
    }
public:
    // constructor
    inline static Ref<RxDisposable> get(const Callable& dispose) { return RxDisposable::wrap(std::make_shared<Disposable>(dispose_cb(dispose))); }
    // set-get
    DISPOSABLE_SETGET

}; // END class RxDisposable

class RxRefCountDisposable : public RxDisposableBase {
    GDCLASS(RxRefCountDisposable, RxDisposableBase)
    RX_WRAPPER(RxRefCountDisposable, RefCountDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        DISPOSABLE_SETGET_BIND(RxRefCountDisposable)
        RX_WRAPPER_CAST_BINDS(RxRefCountDisposable)
        ClassDB::bind_static_method("RxRefCountDisposable", D_METHOD("get", "disp"), &RxRefCountDisposable::get);

        ClassDB::bind_method(D_METHOD("_get_is_primary_disposed"), &RxRefCountDisposable::_get_is_primary_disposed);
        ClassDB::bind_method(D_METHOD("_set_is_primary_disposed", "value"), &RxRefCountDisposable::_set_is_primary_disposed);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_primary_disposed"), "_set_is_primary_disposed", "_get_is_primary_disposed");

        ClassDB::bind_method(D_METHOD("_get_underlying"), &RxRefCountDisposable::_get_underlying);
        ClassDB::bind_method(D_METHOD("_set_underlying", "value"), &RxRefCountDisposable::_set_underlying);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "underlying_disposable"), "_set_underlying", "_get_underlying");

        ClassDB::bind_method(D_METHOD("_get_count"), &RxRefCountDisposable::_get_count);
        ClassDB::bind_method(D_METHOD("_set_count", "value"), &RxRefCountDisposable::_set_count);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "_set_count", "_get_count");

        ClassDB::bind_method(D_METHOD("release"), &RxRefCountDisposable::release);
        ClassDB::bind_method(D_METHOD("get_disposable"), &RxRefCountDisposable::get_disposable);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "disposable"), "", "get_disposable");
    }
public:
    // constructor
    inline static Ref<RxRefCountDisposable> get(Ref<RxDisposableBase> disp) { return RxRefCountDisposable::wrap(RefCountDisposable::get(RxDisposableBase::unwrap(disp))); }
    // set-get
    DISPOSABLE_SETGET
    inline bool _get_is_primary_disposed() { return this->_ptr->is_primary_disposed; }
    inline void _set_is_primary_disposed(bool v) { this->_ptr->is_primary_disposed = v; }
    inline Ref<RxDisposableBase> _get_underlying() { return RxDisposableBase::wrap(this->_ptr->underlying_disposable); }
    inline void _set_underlying(Ref<RxDisposableBase> v) { this->_ptr->underlying_disposable = RxDisposableBase::unwrap(v); }
    inline uint64_t _get_count() { return this->_ptr->count; }
    inline void _set_count(uint64_t v) { this->_ptr->count = v; }
    // additional methods
    inline void release() { this->_ptr->release(); }
    inline Ref<RxDisposableBase> get_disposable() { return RxDisposableBase::wrap(this->_ptr->get_disposable()); }

}; // END class RxRefCountDisposable

class RxScheduledDisposable : public RxDisposableBase {
    GDCLASS(RxScheduledDisposable, RxDisposableBase)
    RX_WRAPPER(RxScheduledDisposable, ScheduledDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxScheduledDisposable)
        ClassDB::bind_method(D_METHOD("lock"), &RxScheduledDisposable::lock); 
        ClassDB::bind_method(D_METHOD("unlock"), &RxScheduledDisposable::unlock); 
        ClassDB::bind_method(D_METHOD("try_lock"), &RxScheduledDisposable::try_lock);
        
        ClassDB::bind_method(D_METHOD("_is_disposed"), &RxScheduledDisposable::_is_disposed);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "", "_is_disposed");

        ClassDB::bind_method(D_METHOD("_get_scheduler"), &RxScheduledDisposable::_get_scheduler);
        ClassDB::bind_method(D_METHOD("_set_scheduler", "value"), &RxScheduledDisposable::_set_scheduler);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scheduler"), "_set_scheduler", "_get_scheduler");
    }
public:
    // constructor
    inline static Ref<RxScheduledDisposable> get(Ref<RxSchedulerBase> scheduler, Ref<RxDisposableBase> disp) { return RxScheduledDisposable::wrap(ScheduledDisposable::get(RxSchedulerBase::unwrap(scheduler), RxDisposableBase::unwrap(disp))); }
    
    inline void lock() { this->_ptr->lock.lock(); } 
    inline void unlock() { this->_ptr->lock.unlock(); } 
    inline bool try_lock() { return this->_ptr->lock.try_lock(); }
    inline bool _is_disposed() { return this->_ptr->is_disposed(); }
    
    inline void _set_scheduler(Ref<RxDisposableBase> scheduler) { this->_ptr->scheduler = RxSchedulerBase::unwrap(scheduler); }
    inline Ref<RxSchedulerBase> _get_scheduler() { return RxSchedulerBase::wrap(this->_ptr->scheduler); }

}; // END class RxScheduledDisposable

class RxCompositeDisposable : public RxDisposableBase {
    GDCLASS(RxCompositeDisposable, RxDisposableBase)
    RX_WRAPPER(RxCompositeDisposable, CompositeDisposable, RxDisposableBase, DisposableBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxCompositeDisposable)
        DISPOSABLE_SETGET_BIND(RxCompositeDisposable)
        ClassDB::bind_static_method("RxCompositeDisposable", D_METHOD("get", "items"), &RxCompositeDisposable::get);

        ClassDB::bind_method(D_METHOD("add", "item"), &RxCompositeDisposable::add);
        ClassDB::bind_method(D_METHOD("remove", "item"), &RxCompositeDisposable::remove);
        ClassDB::bind_method(D_METHOD("clear"), &RxCompositeDisposable::clear);
        ClassDB::bind_method(D_METHOD("contains", "item"), &RxCompositeDisposable::contains);
        ClassDB::bind_method(D_METHOD("to_list"), &RxCompositeDisposable::to_list);
        ClassDB::bind_method(D_METHOD("size"), &RxCompositeDisposable::size);
        ClassDB::bind_method(D_METHOD("is_empty"), &RxCompositeDisposable::empty);

    }
public:
    // constructor
    inline static Ref<RxCompositeDisposable> get(const TypedArray<RxDisposableBase>& items) { 
        return RxCompositeDisposable::wrap(
            std::make_shared<CompositeDisposable>(rx::iterator::to_iterable(items).operator rx::iterable_t()));
    }
    // set-get
    DISPOSABLE_SETGET

    inline void add(Ref<RxDisposableBase> item) { this->_ptr->add(RxDisposableBase::unwrap(item)); }
    inline void remove(Ref<RxDisposableBase> item) { this->_ptr->remove(RxDisposableBase::unwrap(item)); }
    inline void clear() { this->_ptr->clear(); }
    inline bool contains(Ref<RxDisposableBase> item) { return this->_ptr->contains(RxDisposableBase::unwrap(item)); }
    inline TypedArray<RxDisposableBase> to_list() {
        TypedArray<RxDisposableBase> arr; 
        for (auto& item : this->_ptr->to_list()) {
            arr.push_back(RxDisposableBase::wrap(item));
        }
        return arr;
    }
    inline size_t size() { return this->_ptr->size(); }
    inline bool empty() { return this->_ptr->size() == 0; }

}; // END class RxCompositeDisposable

} // END namespace rx
} // END namespace wrapper

#endif // RX_WRAPPER_DISPOSABLE_H