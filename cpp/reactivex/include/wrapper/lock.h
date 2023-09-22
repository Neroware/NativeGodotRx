#ifndef RX_WRAPPER_LOCK_H
#define RX_WRAPPER_LOCK_H

#include "wrapper/abstract.h"

#include "internal/lock.h"
#include "internal/rlock.h"

#include "templates/lock.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::templates;

namespace rx::wrappers {

class RxRecursiveLock : public RxLockBase {
    GDCLASS(RxRecursiveLock, RxLockBase)
    RX_WRAPPER(RxRecursiveLock, RLock, RxLockBase, LockBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxRecursiveLock", D_METHOD("get"), &RxRecursiveLock::get);
        RX_WRAPPER_CAST_BINDS(RxRecursiveLock)
    }

public:
    inline static Ref<RxRecursiveLock> get() {
        return RxRecursiveLock::wrap(std::make_shared<RLock>());
    }

}; // END class RxRecursiveLock

class RxLock : public RxLockBase {
    GDCLASS(RxLock, RxLockBase)
    RX_WRAPPER(RxLock, Lock, RxLockBase, LockBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxLock", D_METHOD("get"), &RxLock::get);
        RX_WRAPPER_CAST_BINDS(RxLock)
    }

public:
    inline static Ref<RxLock> get() {
        return RxLock::wrap(std::make_shared<Lock>());
    }

}; // END class RxLock

/* ================================================================================ */
//                                   TEMPLATES
/* ================================================================================ */

class RxLock_ : public RxLockBase {
    GDCLASS(RxLock_, RxLockBase)
    _RX_WRAPPER(RxLock_, Lock_, RxLockBase, LockBase)

protected:
    static inline void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxLock_)
        ClassDB::bind_method(D_METHOD("_template", "t"), &RxLock_::_template);
    }

public:
    RxLock_() 
        : RxLockBase(std::static_pointer_cast<LockBase>(Lock_::get())), 
        _ptr(std::static_pointer_cast<Lock_>(RxLockBase::getptr())) {}
    
    inline String _to_string() const {
        return "[" + this->_ptr->classname() + ":" + UtilityFunctions::str(
            reinterpret_cast<uint64_t>(this->_ptr.get())) + "]";
    }
    
    inline void _template(Ref<RxLockTemplate_> t) {
        this->_ptr->_template(t);
    }
    
}; // END class RxLock_

} // END namespace rx::wrappers

#endif // RX_WRAPPER_LOCK_H