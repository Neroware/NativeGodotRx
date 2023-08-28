#ifndef RX_TYPING_H
#define RX_TYPING_H

#include <functional>

template<typename KeyT, typename... ArgsT>
using mapper_t = std::function<KeyT(ArgsT...)>;

template<typename KeyT, typename T>
using mapper_indexed_t = mapper_t<KeyT, T, uint64_t>;

template<typename... ArgsT>
using predicate_t = std::function<bool(ArgsT...)>;

template<typename T>
using predicate_indexed_t = predicate_t<T, uint64_t>;

#endif // RX_TYPING_H