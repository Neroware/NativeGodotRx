#ifndef RX_UTILS_H
#define RX_UTILS_H

#include <godot_cpp/variant/callable.hpp>

#include <functional>

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


} // END namespace rx

#endif // RX_UTILS_H