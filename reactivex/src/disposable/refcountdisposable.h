#ifndef RX_DISPOSABLE_REFCOUNTDISPOSABLE_H
#define RX_DISPOSABLE_REFCOUNTDISPOSABLE_H

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


class RefCountDisposable : public DisposableBase {
    GDCLASS(RefCountDisposable, DisposableBase);

public:
    Ref<DisposableBase> underlying_disposable;
    bool is_primary_disposed;
    bool is_disposed;
    Ref<RLock> lock;
    int count;

protected:
	static void _bind_methods();

public:
    RefCountDisposable() : underlying_disposable(Ref<DisposableBase>()), is_primary_disposed(false), is_disposed(false), lock(RLock::Get()), count(0) {}
    RefCountDisposable(Ref<DisposableBase> disposable) : underlying_disposable(disposable), is_primary_disposed(false), is_disposed(false), lock(RLock::Get()), count(0) {}
    ~RefCountDisposable(){}

    static Ref<RefCountDisposable> Get(Ref<DisposableBase> disp);

    void dispose() override;
    void dispose_with(Object* obj) override;

    void release();
    Ref<DisposableBase> get_disposable();

    // Setters and Getters
    Ref<DisposableBase> __get__underlying_disposable__();
    bool __get__is_primary_disposed__();
    void __set__is_disposed__(bool is_disposed);
    bool __get__is_disposed__();
    Ref<RLock> __get__lock__();
    int __get__count__();
};


class InnerDisposable : public DisposableBase {
    GDCLASS(InnerDisposable, DisposableBase);

public:
    Ref<RefCountDisposable> parent;
    bool is_disposed;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    InnerDisposable() : parent(Ref<RefCountDisposable>()), is_disposed(false), lock(RLock::Get()) {}
    InnerDisposable(Ref<RefCountDisposable> parent_) : parent(parent_), is_disposed(false), lock(RLock::Get()) {}
    ~InnerDisposable(){}

    static Ref<InnerDisposable> Get(Ref<RefCountDisposable> parent);

    void dispose() override;
    void dispose_with(Object* obj) override;

    // Setters and Getters
    Ref<RefCountDisposable> __get__parent__();
    void __set__is_disposed__(bool is_disposed);
    bool __get__is_disposed__();
    Ref<RLock> __get__lock__();
};

#endif // RX_DISPOSABLE_REFCOUNTDISPOSABLE_H