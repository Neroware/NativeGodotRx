#ifndef RX_RLOCK_H
#define RX_RLOCK_H

#include <mutex>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/lock.h"

using namespace godot;

class RLock : public LockBase {
    GDCLASS(RLock, LockBase);

private:
    std::recursive_mutex rm;

protected:
	static void _bind_methods();

public:
    RLock(){}
    ~RLock(){}

    static Ref<RLock> Get();

    void lock() override;
    void unlock() override;
    bool try_lock() override;
};

#endif // RX_RLOCK_H