#ifndef RX_UTILS_H
#define RX_UTILS_H

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include <memory>
#include <functional>

#include "abstract/disposable.h"
#include "abstract/observable.h"
#include "abstract/observer.h"
#include "abstract/scheduler.h"
#include "abstract/iterable.h"
#include "abstract/iterator.h"

using namespace godot;

#define VNULL Variant()

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

#define RX_ITERABLE(Iterable, ...) rx::rx_iterable(std::make_shared<Iterable>(__VA_ARGS__))

namespace rx {

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

#define NOT_SET(x) NotSet::is(x)
#define IS_SET(x) !NotSet::is(x)

typedef std::shared_ptr<rx::abstract::DisposableBase> disposable_t;
typedef std::shared_ptr<rx::abstract::ObservableBase> observable_t;
typedef std::shared_ptr<rx::abstract::ObserverBase> observer_t;
typedef std::shared_ptr<rx::abstract::SchedulerBase> scheduler_t;
typedef std::shared_ptr<rx::abstract::IteratorBase> iterator_t;
typedef std::shared_ptr<rx::abstract::IterableBase> iterable_t;

typedef std::list<disposable_t> disposable_list_t;
typedef std::vector<disposable_t> disposable_vec_t;

namespace observable {
    class Observable;
}
typedef std::shared_ptr<rx::observable::Observable> rx_observable_t;
typedef std::list<rx_observable_t> observable_list_t;
typedef std::vector<rx_observable_t> observable_vec_t;

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

template <typename T>
static std::vector<T> insert_front(const T& single, const std::vector<T>& vec) {
    std::vector<T> res(vec.size() + 1);
    res[0] = single;
    for (auto i = 1ul; i < res.size(); i++) {
        res[i] = vec[i - 1];
    }
    return res;
}

} // END namespace rx

#endif // RX_UTILS_H