#ifndef RX_UTILS_H
#define RX_UTILS_H

#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/array.hpp>

#include <functional>

#include "exception/exceptionwrapper.h"
#include "internal/time.h"
#include "cast.h"

using namespace rx::exception;

#define VNULL Variant()
#define CBNULL Callable()
#define UNSET NotSet::value()

#define SUBSCRIBE(observer, scheduler_default) [=](const observer_t& observer, const scheduler_t& scheduler_default) -> disposable_t
#define OP(source) [=](const rx_observable_t& source) -> rx_observable_t
#define ACTION(scheduler__, state) [=](const scheduler_t& scheduler__, const Variant& state = Variant()) -> disposable_t
#define RECURSIVE_ACTION(scheduler__, state, _action) [=](const scheduler_t& scheduler__, const Variant& state, const auto& _action) -> disposable_t
#define RECURSIVE_ACTION_FWD(_action) ACTION(scheduler__, state) { return _action(scheduler__, state, _action); }
#define PERIODIC_ACTION(state) [=](const Variant& state = Variant()) -> Variant

#define ONNEXT_FWD(observer) [observer](const Variant& value) { observer->on_next(value); }
#define ONERROR_FWD(observer) [observer](const error_t& error) { observer->on_error(error); }
#define ONCOMPLETED_FWD(observer) [observer]() { observer->on_completed(); }

#define BIND_SETGET_PROPERTY(Class, name, setter, getter, type) \
    ClassDB::bind_method(D_METHOD(#setter, "v"), &Class::setter); \
    ClassDB::bind_method(D_METHOD(#getter), &Class::getter); \
    ADD_PROPERTY(PropertyInfo(Variant::type, #name), #setter, #getter);
#define BIND_SET_PROPERTY(Class, name, setter, type) \
    ClassDB::bind_method(D_METHOD(#setter, "v"), &Class::setter); \
    ADD_PROPERTY(PropertyInfo(Variant::type, #name), #setter, "");
#define BIND_GET_PROPERTY(Class, name, getter, type) \
    ClassDB::bind_method(D_METHOD(#getter), &Class::getter); \
    ADD_PROPERTY(PropertyInfo(Variant::type, #name), "", #getter);

#define NOT_SET(x) NotSet::is(x)
#define IS_SET(x) !NotSet::is(x)

using namespace godot;

namespace rx {

template<typename RetT, typename... Args>
std::function<RetT(Args...)> from_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](Args... args) -> RetT {
        return cb.callv(Array::make(args...));
    };
}
template<typename... Args>
std::function<void(Args...)> from_void_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](Args... args) -> void {
        cb.callv(Array::make(args...));
    };
}

static auto on_next_cb = from_void_cb<const Variant&>;
static on_error_t on_error_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](const error_t& error) -> void {
        cb.callv(Array::make(RxError::wrap(error)));
    };
}
static auto on_completed_cb = from_void_cb<>;
static auto scheduled_action_cb = from_cb<disposable_t, const scheduler_t&, const godot::Variant&>;
static auto periodic_action_cb = from_cb<Variant, const godot::Variant&>;
static auto dispose_cb = from_void_cb<>;
static auto run_cb = from_cb<Variant>;
static auto notification_handler_cb = from_cb<notification_t, const notification_t&>;
static auto rx_subject_factory_cb = from_cb<rx_subject_t, const scheduler_t&>;
static auto comparer_cb = from_cb<bool, const Variant&, const Variant&>;
static auto sub_comparer_cb = from_cb<double, const Variant&, const Variant&>;
static auto notifier_cb = from_void_cb<const notification_t&>;
static handler_t handler_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](const error_t& error) -> bool {
        return cb.callv(Array::make(RxError::wrap(error)));
    };
}
static std::function<rx_observable_t(const error_t&, const rx_observable_t&)> catch_handler_cb(const Callable& cb) {
    if (cb.is_null()) return nullptr;
    return [cb](const error_t& error, const rx_observable_t&) -> rx_observable_t {
        return cb.callv(Array::make(RxError::wrap(error)));
    };
}
static auto observable_factory_cb = from_cb<rx_observable_t, const scheduler_t&>;
static auto stated_observable_factory_cb = from_cb<rx_observable_t, const Variant&>;
template<typename... Args> static auto predicate_cb = from_cb<bool, const Args&...>;
template<typename RetT, typename... Args> static auto mapper_cb = from_cb<RetT, const Args&...>;
template<typename StateT, typename T> static auto accumulator_cb = from_cb<StateT, const StateT&, const T&>;

class NotSet : public RefCounted {
    GDCLASS(NotSet, RefCounted)
public:
    static bool is(const Variant& v);
    static Ref<NotSet> value();
protected:
    inline static void _bind_methods() {
        ClassDB::bind_static_method("NotSet", D_METHOD("is", "value"), &NotSet::is);
        ClassDB::bind_static_method("NotSet", D_METHOD("value"), &NotSet::value);
    }
};

struct variant_compare {
    bool operator()(const Variant& a, const Variant& b) const {
        return UtilityFunctions::hash(a) < UtilityFunctions::hash(b);
    }
};

/* Utility helper functions */

template <typename CallableT>
static RxList<CallableT> convert_all_cb(const RxList<Callable>& xs, const std::function<CallableT(const godot::Callable&)>& converter) {
    RxList<CallableT> result;
    for (const Callable& cb : xs) {
        result.push_back(converter(cb));
    }
    return result;
}

template<typename T>
static bool all(const std::shared_ptr<T[]>& arr, int n) {
    for (int i = 0; i < n; i++) {
        if (!arr[i]) {
            return false;
        }
    }
    return true;
}
template<typename T>
static bool all(const std::shared_ptr<T[]>& arr, int n, const std::function<bool(const T&)>& pred) {
    for (int i = 0; i < n; i++) {
        if (!pred(arr[i])) {
            return false;
        }
    }
    return true;
}
template<typename T>
static bool all(const T& arr) {
    for (int i = 0; i < arr.size(); i++) {
        if (!arr[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
static Array tuple(const T& values, int n) {
    Array res; 
    for (auto i = 0; i < n; i++){ 
        res.append(values[i]);
    }
    return res;
} 

static time_delta_t get_dt(const Variant& time) {
    if (IS_NUMBER(time)) {
        return DELTA_ZERO + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(time.operator double()));
    }
    if (auto t = DYN_CAST_OR_NULL(time, AbsoluteTime)) {
        return t->t.time_since_epoch();
    }
    if (auto dt = DYN_CAST_OR_NULL(time, RelativeTime)) {
        return dt->dt;
    }
    throw BadArgumentException("The provided argument was not a timespan!");
} 

} // END namespace rx

#endif // RX_UTILS_H