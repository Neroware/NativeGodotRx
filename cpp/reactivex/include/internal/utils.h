#ifndef RX_UTILS_H
#define RX_UTILS_H

#include <godot_cpp/variant/array.hpp>
#include <memory>
#include <functional>

#include "observable/observable.h"

using namespace godot;
using namespace rx::observable;

namespace rx {

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