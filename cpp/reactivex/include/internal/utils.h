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
#define ACTION(scheduler__, state) [=](const std::shared_ptr<SchedulerBase>& scheduler__, const Variant& state = Variant()) -> std::shared_ptr<DisposableBase>
#define RECURSIVE_ACTION(scheduler__, state, _action) [=](const std::shared_ptr<SchedulerBase>& scheduler__, const Variant& state, const auto& _action) -> std::shared_ptr<DisposableBase>
#define RECURSIVE_ACTION_FWD(_action) ACTION(scheduler__, state) { return _action(scheduler__, state, _action); }
#define PERIODIC_ACTION(state) [=](const Variant& state = Variant()) -> Variant

namespace rx {

typedef std::shared_ptr<DisposableBase> disposable_t;
typedef std::shared_ptr<ObservableBase> observable_t;
typedef std::shared_ptr<ObserverBase> observer_t;
typedef std::shared_ptr<SchedulerBase> scheduler_t;

template<typename T>
static bool all(const std::shared_ptr<T[]>& arr, int n) {
    bool allTrue = true;
    for (int i = 0; i < n; i++) {
        if (!arr[i]) {
            allTrue = false;
            break;
        }
    }
    return allTrue;
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