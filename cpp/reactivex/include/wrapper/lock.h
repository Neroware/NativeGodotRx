#ifndef RX_WRAPPER_LOCK_H
#define RX_WRAPPER_LOCK_H

#include "wrapper/wrapper.h"

#include "abstract/lock.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxLockBase : public RefCounted {
    GDCLASS(RxLockBase, RefCounted)
    RX_ABSTRACT_WRAPPER(RxLockBase, LockBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("lock"), &RxLockBase::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxLockBase::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxLockBase::try_lock);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxLockBase::equals);
    }
public:
    void lock();
    void unlock();
    bool try_lock();
};

};

#endif // RX_WRAPPER_LOCK_H