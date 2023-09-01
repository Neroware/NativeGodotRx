#ifndef RX_GODOTRX_H
#define RX_GODOTRX_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "internal/concurrency.h"
#include "internal/iterator.h"
#include "internal/thread.h"
#include "internal/weakkeydictionary.h"
#include "internal/utils.h"

#include "notification.h"

#include "scheduler/currentthreadscheduler.h"
#include "scheduler/immediatescheduler.h"
#include "scheduler/timeoutscheduler.h"
#include "scheduler/scenetreetimeoutscheduler.h"

#define GDRX_SINGLETON_NAME "GDRx"
#define GDRX __GDRxSingleton__::singleton()
// #define GDRX Ref<__GDRxSingleton__>(Engine::get_singleton()->get_singleton(GDRX_SINGLETON_NAME))

using namespace godot;
using namespace rx::scheduler;

namespace rx {

class __GDRxSingleton__ : public RefCounted {
    GDCLASS(__GDRxSingleton__, RefCounted);

public:
    /* Thread manager singleton */
    const std::shared_ptr<ThreadManager> THREAD_MANAGER = std::make_shared<ThreadManager>();
    /* Main thread dummy */
    const Ref<RxThread> MAIN_THREAD = memnew(RxMainThread);

    /* Scheduler singletons */
    weakkey_dictionary<variant_key_t, weakkey_dictionary<variant_key_t, std::shared_ptr<CurrentThreadScheduler>>>CurrentThreadScheduler_global_;
    const std::shared_ptr<_CurrentThreadScheduler_Local> CurrentThreadScheduler_local_ = _CurrentThreadScheduler_Local::get();
    const std::shared_ptr<ImmediateScheduler> ImmediateScheduler_ = ImmediateScheduler::get();
    const std::shared_ptr<TimeoutScheduler> TimeoutScheduler_ = TimeoutScheduler::get();
    std::map<uint8_t, std::shared_ptr<SceneTreeTimeoutScheduler>> SceneTreeTimeoutScheduler_;

    /* Not Set */
    const Ref<NotSet> NOT_SET = memnew(NotSet);

private:
    static __GDRxSingleton__* p_instance;

protected:
	static inline void _bind_methods() {
        ClassDB::bind_method(D_METHOD("iter", "it"), &__GDRxSingleton__::iter);
        ClassDB::bind_method(D_METHOD("to_iterable", "it"), &__GDRxSingleton__::to_iterable);
        ClassDB::bind_method(D_METHOD("foreach", "it", "action"), &__GDRxSingleton__::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "it", "action"), &__GDRxSingleton__::enumerate);
        ClassDB::bind_method(D_METHOD("from_notifier", "handler"), &__GDRxSingleton__::from_notifier);
    }

public:
    inline static __GDRxSingleton__* singleton() {
        return __GDRxSingleton__::p_instance;
    }

    __GDRxSingleton__(){
        // Singleton values
        {
            __GDRxSingleton__::p_instance = this;
            CurrentThreadScheduler::cls = std::make_shared<variant_key_t>("CurrentThreadScheduler");
        }

        // Insert Main Thread
        {
            std::unique_lock<std::shared_mutex> writeLock(this->THREAD_MANAGER->thread_registry.first);
            auto main_thread_id = std::this_thread::get_id();
            this->THREAD_MANAGER->thread_registry.second[main_thread_id] = MAIN_THREAD;
        }

        // Scheduler Singletons
        {
            for (auto i = 0; i < 8; i++) {
                this->SceneTreeTimeoutScheduler_[i] = SceneTreeTimeoutScheduler::get(i & 0b100, i & 0b10, i & 0b1);
            }
        }
    }

    ~__GDRxSingleton__(){}

    inline Ref<RxIteratorBase> iter(const Variant& iterable) {
        return rx::iterator::iter(iterable);
    }
    inline Ref<RxIterableBase> to_iterable(const Variant& iterable) {
        return rx::iterator::to_iterable(iterable);
    }
    inline void foreach(const Variant& it, const Callable& what) {
        this->iter(it)->foreach(what);
    }
    inline void enumerate(const Variant& it, const Callable& what) {
        this->iter(it)->enumerate(what);
    }
    inline Ref<RxObserverBase> from_notifier(const Callable& handler) {
        return RxObserverBase::wrap(rx::from_notifier(notification_handler_cb(handler)));
    }
};

inline __GDRxSingleton__* __GDRxSingleton__::p_instance = nullptr;

}; // END namespace rx

#endif // RX_GODOTRX_H