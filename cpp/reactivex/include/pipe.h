#ifndef RX_PIPE_H
#define RX_PIPE_H

namespace rx::pipe {

template <typename Ops>
auto compose(Ops op) {
    return op;
}
template <typename FirstOp, typename... Ops>
auto compose(FirstOp firstOp, Ops... ops) {
    return [=](auto x) {
        return compose(ops...)(firstOp(x));
    };
}

template <typename T, typename... Ops>
auto pipe(T value, Ops... fns) {
    return compose(fns...)(value);
}

template <typename T, typename... Ops>
auto operator|(T value, Ops... fns) {
    return pipe(value, fns...);
}

}

#endif // RX_PIPE_H