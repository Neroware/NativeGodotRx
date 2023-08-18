#ifndef RX_TYPING_H
#define RX_TYPING_H

#include <functional>

template<typename KeyT, typename... ArgsT>
using mapper_t = std::function<KeyT(ArgsT...)>;

template<typename... ArgsT>
using predicate_t = std::function<bool(ArgsT...)>;

#endif // RX_TYPING_H