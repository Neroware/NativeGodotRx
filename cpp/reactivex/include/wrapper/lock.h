#ifndef RX_WRAPPER_LOCK_H
#define RX_WRAPPER_LOCK_H

#include "wrapper/abstract.h"

#include "internal/lock.h"
#include "internal/rlock.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;


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

} // END namespace rx::wrappers

#endif // RX_WRAPPER_LOCK_H