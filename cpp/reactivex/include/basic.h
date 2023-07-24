#ifndef RX_BASIC_H
#define RX_BASIC_H

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/callable.hpp>

#include <chrono>
#include <functional>
#include <exception>

#include <exception/exceptionwrapper.h>

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

typedef std::function<void(const Variant&)> on_next_t;
typedef std::function<void(const std::exception&)> on_error_t;
typedef std::function<void()> on_completed_t;

typedef std::function<void()> dispose_t;
typedef std::function<void(const Variant&, const Variant&)> action_t;

static on_next_t on_next_cb(const Callable& cb) {
    return on_next_t([cb](const Variant& item){
        return cb.callv(Array::make(item));
    });
}
static on_error_t on_error_cb(const Callable& cb) {
    return on_error_t([cb](const std::exception& e){
        return cb.callv(Array::make(rx::exception::RxError::wrap(e)));
    });
}
static on_completed_t on_completed_cb(const Callable& cb) {
    return on_completed_t([cb](){
        return cb.callv(Array());
    });
}

static dispose_t dispose_cb(const Callable& cb) {
    return dispose_t([cb](){
        return cb.callv(Array());
    });
}
static action_t action_cb(const Callable& cb) {
    return action_t([cb](const Variant& scheduler, const Variant& state = Variant()){
        return cb.callv(Array::make(scheduler, state));
    });
}

#define DEFAULT_ON_NEXT rx::basic::noop<const Variant&>
#define DEFAULT_ON_ERROR rx::basic::noop<const std::exception&>
#define DEFAULT_ON_COMPLETED rx::basic::noop<>
#define DEFAULT_DISPOSE rx::basic::noop<>

#define DYN_CAST(x, cls) Object::cast_to<cls>(x)
#define DYN_CAST_OR_NULL(x, cls) x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls) ? Object::cast_to<cls>(x) : nullptr
#define IS_CLASS(x, cls) (x.get_type() == Variant::Type::OBJECT && Object::cast_to<Object>(x)->is_class(#cls))

}; // END namespace rx

#endif // RX_BASIC_H