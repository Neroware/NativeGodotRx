#ifndef RX_UTILS_H
#define RX_UTILS_H

#include <godot_cpp/variant/array.hpp>
#include <memory>
#include <functional>

#include "abstract/disposable.h"
#include "abstract/observable.h"
#include "abstract/observer.h"
#include "abstract/scheduler.h"

using namespace godot;

#define SUBSCRIBE(observer, scheduler_default) [=](const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler_default) -> std::shared_ptr<DisposableBase>
#define OP(source) [=](const std::shared_ptr<Observable>& source) -> std::shared_ptr<Observable>
#define ACTION(scheduler__, state) [=](const std::shared_ptr<SchedulerBase>& scheduler__, const Variant& state = Variant()) -> std::shared_ptr<DisposableBase>
#define RECURSIVE_ACTION(scheduler__, state, _action) [=](const std::shared_ptr<SchedulerBase>& scheduler__, const Variant& state, const auto& _action) -> std::shared_ptr<DisposableBase>
#define RECURSIVE_ACTION_FWD(_action) ACTION(scheduler__, state) { return _action(scheduler__, state, _action); }
#define PERIODIC_ACTION(state) [=](const Variant& state = Variant()) -> Variant

#define ONNEXT_FWD(observer) [observer](const Variant& value) { observer->on_next(value); }
#define ONERROR_FWD(observer) [observer](const std::exception_ptr& error) { observer->on_error(error); }
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

namespace rx {

typedef std::shared_ptr<DisposableBase> disposable_t;
typedef std::shared_ptr<ObservableBase> observable_t;
typedef std::shared_ptr<ObserverBase> observer_t;
typedef std::shared_ptr<SchedulerBase> scheduler_t;

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

} // END namespace rx

#endif // RX_UTILS_H