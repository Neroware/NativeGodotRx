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
#include "internal/tuple.h"
#include "internal/iterator.h"

#include "scheduler/currentthreadscheduler.h"
#include "scheduler/immediatescheduler.h"
#include "scheduler/timeoutscheduler.h"

#define GDRX_SINGLETON_NAME "GDRx"
#define GDRX REF_CAST(Engine::get_singleton()->get_singleton(GDRX_SINGLETON_NAME), __GDRxSingleton__)

using namespace godot;

class __GDRxSingleton__ : public RefCounted {
    GDCLASS(__GDRxSingleton__, RefCounted);

public:
    /* Thread registry singleton */
    std::pair<std::shared_mutex, std::unordered_map<std::thread::id, Ref<RxThread>>> thread_registry;
    /* Main thread dummy */
    const Ref<RxThread> MAIN_THREAD = memnew(RxMainThread);

    /* Scheduler singletons */
    const Ref<RefCounted> CurrentThreadScheduler_cls_ = memnew(RefCounted);
    Ref<WeakKeyDictionary> CurrentThreadScheduler_global_;
    Ref<_CurrentThreadScheduler_Local> CurrentThreadScheduler_local_;
    Ref<ImmediateScheduler> ImmediateScheduler_;
    Ref<TimeoutScheduler> TimeoutScheduler_;

private:

protected:
	static inline void _bind_methods() {
        {
		    MethodInfo mi;
		    mi.arguments.push_back(PropertyInfo(Variant::NIL, "data"));
		    mi.name = "tuple";
		    ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "tuple", &__GDRxSingleton__::tuple, mi);
	    }
        ClassDB::bind_method(D_METHOD("iter", "it"), &__GDRxSingleton__::iter);
        ClassDB::bind_method(D_METHOD("foreach", "it", "cb"), &__GDRxSingleton__::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "it", "cb"), &__GDRxSingleton__::enumerate);
    }

public:
    __GDRxSingleton__(){
        // Insert Main Thread
        {
            std::unique_lock<std::shared_mutex> writeLock(this->thread_registry.first);
            auto main_thread_id = std::this_thread::get_id();
            this->thread_registry.second[main_thread_id] = MAIN_THREAD;
        }

        // Scheduler Singletons
        {
            this->CurrentThreadScheduler_global_ = WeakKeyDictionary::Get();
            this->CurrentThreadScheduler_local_ = _CurrentThreadScheduler_Local::Get();
            this->ImmediateScheduler_ = ImmediateScheduler::Get();
            this->TimeoutScheduler_ = TimeoutScheduler::Get();
        }
    }

    ~__GDRxSingleton__(){

    }

    Ref<Tuple> tuple(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
        Array data;
        for (auto i = 0ul; i < arg_count; i++) {
            data.append(*args[i]);
        }
        return Tuple::Get(data);
    }

    Ref<Iterator> iter(const Variant& iterable) {
        return create_iterator(iterable);
    }
    void foreach(const Variant& it, const Callable& what) {
        this->iter(it)->foreach(what);
    }
    void enumerate(const Variant& it, const Callable& what) {
        this->iter(it)->enumerate(what);
    }
};

#endif // RX_GODOTRX_H