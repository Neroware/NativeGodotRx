#ifndef RX_GODOTRX_H
#define RX_GODOTRX_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <mutex>
#include <shared_mutex>

#include "internal/iterator.h"
#include "internal/thread.h"

#define GDRX_SINGLETON_NAME "GDRx"
#define GDRX Ref<__GDRxSingleton__>(Engine::get_singleton()->get_singleton(GDRX_SINGLETON_NAME))

using namespace godot;

namespace rx {

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
        ClassDB::bind_method(D_METHOD("iter", "it"), &__GDRxSingleton__::iter);
        ClassDB::bind_method(D_METHOD("foreach", "it", "action"), &__GDRxSingleton__::foreach);
        ClassDB::bind_method(D_METHOD("enumerate", "it", "action"), &__GDRxSingleton__::enumerate);
    }

public:
    __GDRxSingleton__(){
        
    }

    ~__GDRxSingleton__(){

    }

    inline Ref<RxIterator> iter(const Variant& iterable) {
        return rx::iterator::iter(iterable);
    }
    inline void foreach(const Variant& it, const Callable& what) {
        this->iter(it)->foreach(what);
    }
    inline void enumerate(const Variant& it, const Callable& what) {
        this->iter(it)->enumerate(what);
    }
};

}; // END namespace rx

#endif // RX_GODOTRX_H