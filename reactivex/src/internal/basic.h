#ifndef RX_BASIC_H
#define RX_BASIC_H

#include <chrono>
#include <string>
#include <exception>
#include <iostream>

#define CAST_OR_NULL(x, cls) x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls) ? Object::cast_to<cls>(x) : nullptr
#define OBJ_CAST(x, cls) Object::cast_to<cls>(x)
#define REF_CAST(x, cls) Ref<cls>(Object::cast_to<cls>(x))

namespace basic {

template <typename... T>
void noop(T... args) {}

template <typename T>
T identity(T x) {
    return x;
}
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
    std::cout << "Unhandled exception: " << err.what << std::endl;
    exit(1);
}

template <typename... T>
bool default_condition(T... args) {
    return true;
}

} // END Namespace basic

#endif // RX_BASIC_H