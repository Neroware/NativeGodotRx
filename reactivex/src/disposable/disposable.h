#ifndef RX_DISPOSABLE_DISPOSABLE_H
#define RX_DISPOSABLE_DISPOSABLE_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>

#include "abstract/disposable.h"
#include "internal/basic.h"
#include "internal/rlock.h"
#include "exception/exception.h"

using namespace godot;

class Disposable : public DisposableBase {
    GDCLASS(Disposable, DisposableBase);

public:
    bool is_disposed;
    Callable action;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    Disposable(){}
    ~Disposable(){}

    static Ref<Disposable> Get(const Callable& action);
    static Ref<Disposable> Empty();

    void dispose() override;
    void dispose_with(Object* obj) override;

    // Setters and Getters
    bool __get__is_disposed__();
    void __set__is_disposed__(bool is_disposed);
    Ref<RLock> __get__lock__();
    Callable __get__action__();
    void __set__action__(const Callable& action);
};

#endif // RX_DISPOSABLE_DISPOSABLE_H