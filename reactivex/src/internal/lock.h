#ifndef RX_LOCK_H
#define RX_LOCK_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <condition_variable>
#include <mutex>

#include "abstract/lock.h"

using namespace godot;

class Lock : public LockBase {
    GDCLASS(Lock, LockBase);

private:
    bool flag;
    std::mutex m;
    std::condition_variable cv;

protected:
	static void _bind_methods();

public:
    Lock(){}
    ~Lock(){}

    static Ref<Lock> Get();

    void lock() override;
    void unlock() override;
    bool try_lock() override;
};

#endif // RX_LOCK_H