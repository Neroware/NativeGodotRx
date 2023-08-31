#ifndef RX_TYPING_H
#define RX_TYPING_H

#include <godot_cpp/variant/callable.hpp>
#include <functional>

template<typename KeyT, typename... ArgsT>
using mapper_t = std::function<KeyT(const ArgsT&...)>;

template<typename KeyT, typename T>
using mapper_indexed_t = mapper_t<KeyT, T, uint64_t>;

template<typename... ArgsT>
using predicate_t = std::function<bool(const ArgsT&...)>;

template<typename T>
using predicate_indexed_t = predicate_t<T, uint64_t>;

template<typename T>
using comparer_t = std::function<bool(const T&, const T&)>;

template<typename T>
using sub_comparer_t = std::function<int(const T&, const T&)>;

template<typename StateT, typename T>
using accumulator_t = std::function<StateT(const StateT&, const T&)>;

template<typename KeyT, typename... ArgsT>
mapper_t<KeyT, ArgsT...> mapper_cb(const godot::Callable& mapper) {
    if (mapper.is_null()) {
        return nullptr;
    }
    return [mapper](const ArgsT&... v) -> KeyT { 
        return mapper.callv(Array::make(v...)); 
    };
}
template<typename... ArgsT>
predicate_t<ArgsT...> predicate_cb(const godot::Callable& predicate) {
    if (predicate.is_null()) {
        return nullptr;
    }
    return [predicate](const ArgsT&... v) -> bool { 
        return predicate.callv(Array::make(v...)); 
    };
}
template<typename T>
comparer_t<T> comparer_cb(const godot::Callable& comparer) {
    if (comparer.is_null()) {
        return nullptr;
    }
    return [comparer](const T& v1, const T& v2) -> bool {
        return comparer.callv(Array::make(v1, v2));
    };
}
template<typename StateT, typename T>
accumulator_t<StateT, T> accumulator_cb(const godot::Callable& accumulator) {
    if (accumulator.is_null()) {
        return nullptr;
    }
    return [accumulator](const StateT& v1, const T& v2) -> StateT {
        return accumulator.callv(Array::make(v1, v2));
    };
}

#endif // RX_TYPING_H