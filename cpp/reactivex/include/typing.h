#ifndef RX_TYPING_H
#define RX_TYPING_H

#include <functional>

template<typename KeyT, typename... ArgsT>
using mapper_t = std::function<KeyT(ArgsT...)>;

template<typename... ArgsT>
using predicate_t = std::function<bool(ArgsT...)>;

template<typename T>
using predicate_indexed_t = predicate_t<T, uint64_t>;

#endif // RX_TYPING_H