#ifndef RX_GODOTRX_H
#define RX_GODOTRX_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <shared_mutex>
#include <thread>

#include "internal/basic.h"
#include "internal/weakkeydictionary.h"
#include "internal/thread.h"

#define GDRX REF_CAST(Engine::get_singleton()->get_singleton(__GDRxSingleton__::get_class_static()), __GDRxSingleton__)

using namespace godot;

class __GDRxSingleton__ : public RefCounted {
    GDCLASS(__GDRxSingleton__, RefCounted);

public:
    /* Thread registry singleton */
    std::pair<std::shared_mutex, std::unordered_map<std::thread::id, Ref<RxThread>>> thread_registry;
    /* Main thread dummy */
    const Ref<RxThread> MAIN_THREAD = memnew(RxMainThread);

private:

protected:
	static inline void _bind_methods() {

    }

public:
    __GDRxSingleton__(){
        {
            // Insert Main Thread
            std::unique_lock<std::shared_mutex> writeLock(this->thread_registry.first);
            auto main_thread_id = std::this_thread::get_id();
            this->thread_registry.second[main_thread_id] = MAIN_THREAD;
        }
    }

    ~__GDRxSingleton__(){}

};

#endif // RX_GODOTRX_H