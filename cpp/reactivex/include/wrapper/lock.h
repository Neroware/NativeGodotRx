#ifndef RX_WRAPPER_LOCK_H
#define RX_WRAPPER_LOCK_H

#include "wrapper/wrapper.h"

#include "abstract/lock.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxLock : public RefCounted {
    GDCLASS(RxLock, RefCounted)
    RX_ABSTRACT_WRAPPER(RxLock, LockBase)

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("lock"), &RxLock::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxLock::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxLock::try_lock);
        ClassDB::bind_method(D_METHOD("equals", "other"), &RxLock::equals);
    }
public:
    void lock();
    void unlock();
    bool try_lock();
};

};

#endif // RX_WRAPPER_LOCK_H