#ifndef RX_DISPOSABLE_BOOLEANDISPOSABLE_H
#define RX_DISPOSABLE_BOOLEANDISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "abstract/disposable.h"
#include "internal/rlock.h"
#include "internal/basic.h"
#include "exception/exception.h"

using namespace godot;

class BooleanDisposable : public DisposableBase {
    GDCLASS(BooleanDisposable, DisposableBase);

public:
    bool is_disposed;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    BooleanDisposable() : is_disposed(false), lock(RLock::Get()) {}
    ~BooleanDisposable(){}

    static Ref<BooleanDisposable> Get();

    void dispose() override;
    void dispose_with(Object* obj) override;

    // Setters and Getters
    bool __get__is_disposed__();
    void __set__is_disposed__(bool is_disposed);
    Ref<RLock> __get__lock__();
};

#endif // RX_DISPOSABLE_BOOLEANDISPOSABLE_H