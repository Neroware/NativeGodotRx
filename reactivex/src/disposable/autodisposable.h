#ifndef RX_DISPOSABLE_AUTODISPOSABLE_H
#define RX_DISPOSABLE_AUTODISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"

#include "internal/basic.h"
#include "internal/rlock.h"

#include <functional>

using namespace godot;

class AutoDisposable : public DisposableBase {
    GDCLASS(AutoDisposable, DisposableBase);

    using dispose_callback = std::function<void()>;

private:
    dispose_callback _on_dispose;
    Ref<RLock> _lock;
    bool _is_disposed;

protected:

	static void _bind_methods();

public:
    AutoDisposable() : _on_dispose(basic::noop<>), _lock(RLock::Get()), _is_disposed(false) {}
    AutoDisposable(dispose_callback on_dispose) : _on_dispose(on_dispose), _lock(RLock::Get()), _is_disposed(false) {} 
    ~AutoDisposable(){}

    static Ref<AutoDisposable> Get(const Callable& on_dispose);

    static void Add(Object* obj, Ref<DisposableBase> disp);

    void dispose() override;
    void dispose_with(Object* obj) override;

    void _notification(int p_what);
};

#endif // RX_DISPOSABLE_AUTODISPOSABLE_H