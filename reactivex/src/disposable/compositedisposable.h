#ifndef RX_DISPOSABLE_COMPOSITEDISPOSABLE_H
#define RX_DISPOSABLE_COMPOSITEDISPOSABLE_H

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

class CompositeDisposable : public DisposableBase {
    GDCLASS(CompositeDisposable, DisposableBase);

public:
    TypedArray<DisposableBase> disposable;
    bool is_disposed;
    Ref<RLock> lock;

protected:
	static void _bind_methods();

public:
    CompositeDisposable() : disposable(TypedArray<DisposableBase>()), is_disposed(false), lock(RLock::Get()) {}
    CompositeDisposable(const TypedArray<DisposableBase>& items) : disposable(items), is_disposed(false), lock(RLock::Get()) {}
    ~CompositeDisposable(){}

    static Ref<CompositeDisposable> Get(const TypedArray<DisposableBase>& items = TypedArray<DisposableBase>());

    void dispose() override;
    void dispose_with(Object* obj) override;

    void add(Ref<DisposableBase> item);
    bool remove(Ref<DisposableBase> item);
    void clear();
    bool contains(Ref<DisposableBase> item) const;
    TypedArray<DisposableBase> to_list() const;
    int size() const;

    // Setters and Getters
    // Setters and Getters
    bool __get__is_disposed__();
    void __set__is_disposed__(bool is_disposed);
    Ref<RLock> __get__lock__();
};

#endif // RX_DISPOSABLE_COMPOSITEDISPOSABLE_H