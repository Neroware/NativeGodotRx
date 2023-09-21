#ifndef RX_WRAPPER_OBSERVABLE_H
#define RX_WRAPPER_OBSERVABLE_H

#include "wrapper/abstract.h"
#include "wrapper/disposable.h"
#include "wrapper/subject.h"

#include "observable/connectableobservable.h"
#include "observable/groupedobservable.h"
#include "observable/reactiveproperty.h"
#include "observable/readonlyreactiveproperty.h"

#include "basic.h"

#include <godot_cpp/classes/rendering_device.hpp>
#include <godot_cpp/classes/http_request.hpp>

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;
using namespace rx::observable;

namespace rx {
namespace wrappers {

class RxConnectableObservable;
class RxGroupedObservable;

class RxObservable : public RxObservableBase {
    GDCLASS(RxObservable, RxObservableBase)
    RX_WRAPPER(RxObservable, Observable, RxObservableBase, ObservableBase)

public:
#include "observable/definitions.inc"

protected:
    static void _bind_methods();
};

class RxConnectableObservable : public RxObservable {
    GDCLASS(RxConnectableObservable, RxObservable)
    RX_WRAPPER(RxConnectableObservable, ConnectableObservable, RxObservable, Observable)

public:
    inline static Ref<RxConnectableObservable> get(Ref<RxObservableBase> source, Ref<RxSubjectBase> subject) {
        return RxConnectableObservable::wrap(ConnectableObservable::get(
            RxObservableBase::unwrap(source),
            RxSubjectBase::unwrap(source)
        ));
    }
    inline Ref<RxDisposableBase> connect(Ref<RxSchedulerBase> scheduler = Ref<RxSchedulerBase>()) {
        return RxDisposableBase::wrap(this->_ptr->connect(
            RxSchedulerBase::unwrap(scheduler)
        ));
    }
    inline Ref<RxObservable> auto_connect(uint64_t subscriber_count = 1) {
        return RxObservable::wrap(this->_ptr->auto_connect(subscriber_count));
    }
    // connectable/_refcount.h
    Ref<RxObservable> ref_count();

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxConnectableObservable)
        ClassDB::bind_static_method("RxConnectableObservable", D_METHOD("get", "source", "subject"), &RxConnectableObservable::get);
        ClassDB::bind_method(D_METHOD("connected", "scheduler"), &RxConnectableObservable::connect, DEFVAL(Ref<RxSchedulerBase>()));
        ClassDB::bind_method(D_METHOD("auto_connect", "subscriber_count"), &RxConnectableObservable::auto_connect, DEFVAL(1));
        ClassDB::bind_method(D_METHOD("ref_count"), &RxConnectableObservable::ref_count);
    }
};


class RxGroupedObservable : public RxObservable {
    GDCLASS(RxGroupedObservable, RxObservable)
    RX_WRAPPER(RxGroupedObservable, GroupedObservable, RxObservable, Observable)

public:
    inline static Ref<RxGroupedObservable> get(const Variant& key, Ref<RxObservable> underlying_observable, Ref<RxRefCountDisposable> merged_disposable = Ref<RxRefCountDisposable>()) {
        return RxGroupedObservable::wrap(GroupedObservable::get(
            key,
            RxObservable::unwrap(underlying_observable),
            RxRefCountDisposable::unwrap(merged_disposable)
        ));
    }

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxGroupedObservable)
        ClassDB::bind_static_method("RxGroupedObservable", D_METHOD("get", "key", "underlying_observable", "merged_disposable"), &RxGroupedObservable::get, DEFVAL(Ref<RxRefCountDisposable>()));
    }
};


class RxReactiveProperty : public RxObservable {
    GDCLASS(RxReactiveProperty, RxObservable)
    RX_WRAPPER_DEFAULT_CONSTRUCTOR(RxReactiveProperty)
    _RX_WRAPPER(RxReactiveProperty, ReactiveProperty, RxObservable, Observable)

public:
    inline static Ref<RxReactiveProperty> get(const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true, Ref<RxObservable> source = VNULL) {
        return ReactiveProperty::get(
            initial_value_, 
            distinct_until_changed_, 
            raise_latest_value_on_subscribe_,
            source
        );
    }
    inline String _to_string() const {
        return this->_ptr->to_string();
    }
    inline Variant _get_value() const {
        return this->_ptr->_get_value();
    }
    inline void _set_value(const Variant& value) {
        this->_ptr->_set_value(value);
    }
    inline bool equals(const Variant& other) const {
        return this->_ptr->equals(other);
    }
    inline void dispose() {
        this->_ptr->dispose();
    }
    inline bool _get_is_disposed() {
        return this->_ptr->is_disposed;
    }
    inline void _set_is_disposed(bool is_disposed_) {
        this->_ptr->is_disposed = is_disposed_;
    }
    inline Ref<RxReadOnlyReactiveProperty> to_readonly() {
        return this->_ptr->to_readonly();
    }
    inline static Ref<RxReactiveProperty> FromGetSet(
        const Callable& getter,
        const Callable& setter,
        bool distinct_until_changed = true,
        bool raise_latest_value_on_subscribe = true
    ) {
        return ReactiveProperty::from_get_set(
            from_cb<Variant>(getter),
            from_void_cb<const Variant&>(setter),
            distinct_until_changed,
            raise_latest_value_on_subscribe
        );
    }
    inline static Ref<RxReactiveProperty> FromMember(
        const Variant& target,
        const StringName& member_name,
        const Callable& convert = CBNULL,
        const Callable& convert_back = CBNULL,
        bool distinct_until_changed = true, 
        bool raise_latest_value_on_subscribe = true
    ) {
        return ReactiveProperty::from_member(
            target,
            member_name,
            from_cb<Variant, const Variant&>(convert),
            from_cb<Variant, const Variant&>(convert_back),
            distinct_until_changed,
            raise_latest_value_on_subscribe
        );
    }
    inline static Ref<RxReadOnlyReactiveProperty> Derived(
        Ref<RxReadOnlyReactiveProperty> p, 
        const Callable& fn
    ) {
        return ReactiveProperty::derived(
            p,
            mapper_cb<Variant, Variant>(fn)
        );
    }
    inline static Ref<RxReadOnlyReactiveProperty> Computed(
        const Variant& sources,
        const Callable& fn
    ) {
        return ReactiveProperty::computed(
            mapper_cb<Variant, Array>(fn),
            iterator::to_iterable(sources)
        );
    }

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxReactiveProperty)
        ClassDB::bind_static_method("RxReactiveProperty", D_METHOD("get", "initial_value", "distinct_until_changed", "raise_latest_value_on_subscribe", "source"), &RxReactiveProperty::get, DEFVAL(true), DEFVAL(true), DEFVAL(VNULL));
        ClassDB::bind_static_method("RxReactiveProperty", D_METHOD("FromGetSet", "getter", "setter", "distinct_until_changed", "raise_latest_value_on_subscribe"), &RxReactiveProperty::FromGetSet, DEFVAL(true), DEFVAL(true));
        ClassDB::bind_static_method("RxReactiveProperty", D_METHOD("FromMember", "target", "member_name", "convert", "convert_back", "distinct_until_changed", "raise_latest_value_on_subscribe"), &RxReactiveProperty::FromMember, DEFVAL(CBNULL), DEFVAL(CBNULL), DEFVAL(true), DEFVAL(true));
        ClassDB::bind_static_method("RxReactiveProperty", D_METHOD("Derived", "p", "fn"), &RxReactiveProperty::Derived);
        ClassDB::bind_static_method("RxReactiveProperty", D_METHOD("Computed", "sources", "fn"), &RxReactiveProperty::Computed);

        ClassDB::bind_method(D_METHOD("_set_value", "value"), &RxReactiveProperty::_set_value);
        ClassDB::bind_method(D_METHOD("_get_value"), &RxReactiveProperty::_get_value);
        ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "Value"), "_set_value", "_get_value");

        ClassDB::bind_method(D_METHOD("_set_is_disposed", "is_disposed"), &RxReactiveProperty::_set_is_disposed);
        ClassDB::bind_method(D_METHOD("_get_is_disposed"), &RxReactiveProperty::_get_is_disposed);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "_set_is_disposed", "_get_is_disposed");

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxReactiveProperty::equals);
        ClassDB::bind_method(D_METHOD("dispose"), &RxReactiveProperty::dispose);
        ClassDB::bind_method(D_METHOD("to_readonly"), &RxReactiveProperty::to_readonly);
    }
};


class RxReadOnlyReactiveProperty : public RxObservable {
    GDCLASS(RxReadOnlyReactiveProperty, RxObservable)
    RX_WRAPPER_DEFAULT_CONSTRUCTOR(RxReadOnlyReactiveProperty)
    _RX_WRAPPER(RxReadOnlyReactiveProperty, ReadOnlyReactiveProperty, RxObservable, Observable)

public:
    inline static Ref<RxReadOnlyReactiveProperty> get(Ref<RxObservable> source, const Variant& initial_value_, bool distinct_until_changed_ = true, bool raise_latest_value_on_subscribe_ = true) {
        return ReadOnlyReactiveProperty::get(
            source,
            initial_value_,
            distinct_until_changed_,
            raise_latest_value_on_subscribe_
        );
    }
    inline String _to_string() const {
        return this->_ptr->to_string();
    }
    inline Variant _get_value() const {
        return this->_ptr->_get_value();
    }
    inline bool equals(const Variant& other) const {
        return this->_ptr->equals(other);
    }
    inline void dispose() {
        this->_ptr->dispose();
    }
    inline bool _get_is_disposed() {
        return this->_ptr->is_disposed;
    }
    inline void _set_is_disposed(bool is_disposed_) {
        this->_ptr->is_disposed = is_disposed_;
    }

protected:
    inline static void _bind_methods() {
        RX_WRAPPER_CAST_BINDS(RxReadOnlyReactiveProperty)
        ClassDB::bind_static_method("RxReadOnlyReactiveProperty", D_METHOD("get", "source", "initial_value", "distinct_until_changed", "raise_latest_value_on_subscribe"), &RxReadOnlyReactiveProperty::get, DEFVAL(true), DEFVAL(true));

        ClassDB::bind_method(D_METHOD("_get_value"), &RxReadOnlyReactiveProperty::_get_value);
        ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "Value"), "", "_get_value");

        ClassDB::bind_method(D_METHOD("_set_is_disposed", "is_disposed"), &RxReadOnlyReactiveProperty::_set_is_disposed);
        ClassDB::bind_method(D_METHOD("_get_is_disposed"), &RxReadOnlyReactiveProperty::_get_is_disposed);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_disposed"), "_set_is_disposed", "_get_is_disposed");

        ClassDB::bind_method(D_METHOD("equals", "other"), &RxReadOnlyReactiveProperty::equals);
        ClassDB::bind_method(D_METHOD("dispose"), &RxReadOnlyReactiveProperty::dispose);
    }
};


} // END namespace wrapper
} // END namespace rx

#endif // RX_WRAPPER_OBSERVABLE_H