#ifndef RX_BASIC_H
#define RX_BASIC_H

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/callable.hpp>

#include <chrono>
#include <functional>
#include <exception>

#include "exception/exceptionwrapper.h"

using namespace godot;

namespace rx {

namespace basic {

template <typename... T>
void noop(T... args) {}

template <typename T0, typename... T1>
T0 identity(T0 x, T1... args) {
    return x;
}

template <typename T>
T now() {
    return std::chrono::system_clock::now();
}

template <typename T>
bool default_comparer(T x, T y) {
    return x == y;
}

template <typename T>
T default_sub_comparer(T x, T y) {
    return x - y;
}

template <typename T>
T default_key_serializer(T x) {
    return std::string(reinterpret_cast<uint64_t>(&x));
}

template <typename T>
void default_error(T err) {
    throw err;
}

template <typename T>
void default_crash(T err) {
    UtilityFunctions::print("Unhandled exception: ", err.what);
    exit(1);
}

template <typename... T>
bool default_condition(T... args) {
    return true;
}

}; // END namespace basic

}; // END namespace rx

#endif // RX_BASIC_H