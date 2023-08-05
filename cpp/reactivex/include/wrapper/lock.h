#ifndef RX_WRAPPER_LOCK_H
#define RX_WRAPPER_LOCK_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "abstract/lock.h"
#include "exception/exception.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx::wrappers {

class RxLock : public RefCounted {
    GDCLASS(RxLock, RefCounted)

private:
    std::shared_ptr<LockBase> _ptr;

public:
    RxLock() { throw NotImplementedException(); }
    RxLock(const std::shared_ptr<LockBase>& lock) : _ptr(lock) {}
    ~RxLock(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("lock"), &RxLock::lock);
        ClassDB::bind_method(D_METHOD("unlock"), &RxLock::unlock);
        ClassDB::bind_method(D_METHOD("try_lock"), &RxLock::try_lock);

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxLock::equals);
    }

public:
    static inline Ref<RxLock> wrap(const std::shared_ptr<LockBase>& lock) {
        return memnew(RxLock(lock));
    }
    inline std::shared_ptr<LockBase> unwrap() const { return this->_ptr; }

    void lock();
    void unlock();
    bool try_lock();

    inline String _to_string() { return "[RxLock:" + UtilityFunctions::str(reinterpret_cast<uint64_t>(this->_ptr.get())) + "]"; }
    inline bool equals(Ref<RxLock> other) { return this->_ptr.get() == other->_ptr.get(); }

};

};

#endif // RX_WRAPPER_LOCK_H