#ifndef RX_PIPE_H
#define RX_PIPE_H

namespace rx::pipe {

template <typename... Args>
auto compose(Args... ops) {
    return [=](auto x) {
        return (ops(x), ...);
    };
}

template <typename T, typename... Args>
auto pipe(T value, Args... fns) {
    return compose(fns...)(value);
}

}

#endif // RX_PIPE_H