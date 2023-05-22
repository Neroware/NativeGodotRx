#ifndef RX_DISPOSABLE_SERIALDISPOSABLE_H
#define RX_DISPOSABLE_SERIALDISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"
#include "internal/basic.h"
#include "internal/rlock.h"
#include "exception/exception.h"

using namespace godot;

class SerialDisposable : public DisposableBase {
    GDCLASS(SerialDisposable, DisposableBase);

public:
    Ref<DisposableBase> current;
    bool is_disposed;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    SerialDisposable() : is_disposed(false), lock(RLock::Get()) {}
    ~SerialDisposable(){}

    static Ref<SerialDisposable> Get();

    void dispose() override;
    void dispose_with(Object* obj) override;

    Ref<DisposableBase> get_disposable();
    void set_disposable(Ref<DisposableBase> value);

    // Setters and Getters
    bool __get__is_disposed__();
    void __set__is_disposed__(bool is_disposed);
    Ref<RLock> __get__lock__();
};

#endif // RX_DISPOSABLE_SERIALDISPOSABLE_H