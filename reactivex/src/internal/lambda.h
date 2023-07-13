#ifndef RX_LAMBDA_H
#define RX_LAMBDA_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <functional>
#include <cassert>

using namespace godot;

#define VARIANT1 const Variant&
#define VARIANT2 VARIANT1, VARIANT1
#define VARIANT3 VARIANT2, VARIANT1
#define VARIANT4 VARIANT3, VARIANT1
#define VARIANT5 VARIANT4, VARIANT1
#define VARIANT6 VARIANT5, VARIANT1
#define VARIANT7 VARIANT6, VARIANT1
#define VARIANT8 VARIANT7, VARIANT1
#define VARIANT9 VARIANT8, VARIANT1
#define VARIANT10 VARIANT9, VARIANT1
#define VARIANT11 VARIANT10, VARIANT1
#define VARIANT12 VARIANT11, VARIANT1
#define VARIANT13 VARIANT12, VARIANT1
#define VARIANT14 VARIANT13, VARIANT1
#define VARIANT15 VARIANT14, VARIANT1

#define VOID_FUN0 std::function<void()>
#define VOID_FUN1 std::function<void(VARIANT1)>
#define VOID_FUN2 std::function<void(VARIANT2)>
#define VOID_FUN3 std::function<void(VARIANT3)>
#define VOID_FUN4 std::function<void(VARIANT4)>
#define VOID_FUN5 std::function<void(VARIANT5)>
#define VOID_FUN6 std::function<void(VARIANT6)>
#define VOID_FUN7 std::function<void(VARIANT7)>
#define VOID_FUN8 std::function<void(VARIANT8)>
#define VOID_FUN9 std::function<void(VARIANT9)>
#define VOID_FUN10 std::function<void(VARIANT10)>
#define VOID_FUN11 std::function<void(VARIANT11)>
#define VOID_FUN12 std::function<void(VARIANT12)>
#define VOID_FUN13 std::function<void(VARIANT13)>
#define VOID_FUN14 std::function<void(VARIANT14)>
#define VOID_FUN15 std::function<void(VARIANT15)>

#define VARIANT_FUN0 std::function<Variant()>
#define VARIANT_FUN1 std::function<Variant(VARIANT1)>
#define VARIANT_FUN2 std::function<Variant(VARIANT2)>
#define VARIANT_FUN3 std::function<Variant(VARIANT3)>
#define VARIANT_FUN4 std::function<Variant(VARIANT4)>
#define VARIANT_FUN5 std::function<Variant(VARIANT5)>
#define VARIANT_FUN6 std::function<Variant(VARIANT6)>
#define VARIANT_FUN7 std::function<Variant(VARIANT7)>
#define VARIANT_FUN8 std::function<Variant(VARIANT8)>
#define VARIANT_FUN9 std::function<Variant(VARIANT9)>
#define VARIANT_FUN10 std::function<Variant(VARIANT10)>
#define VARIANT_FUN11 std::function<Variant(VARIANT11)>
#define VARIANT_FUN12 std::function<Variant(VARIANT12)>
#define VARIANT_FUN13 std::function<Variant(VARIANT13)>
#define VARIANT_FUN14 std::function<Variant(VARIANT14)>
#define VARIANT_FUN15 std::function<Variant(VARIANT15)>

#define FUNCTOR(N) functor([fun](const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) -> Variant { std::apply(fun, FunctionCaller::to_tuple<N>(args)); return Variant(); })
#define RET_FUNCTOR(N) functor([fun](const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) -> Variant { return std::apply(fun, FunctionCaller::to_tuple<N>(args)); })


class RxLambda : public RefCounted {
    GDCLASS(RxLambda, RefCounted);

private:
/**
 * I actually tried to make a hacky way creating a collection of functions using an std::map mapping to 
 * the corresponding caller but I was not able to pull it off... In theory this could work because
 * then we only have compile time sizes but I do not know for sure and I wasted way to much time on this...
 */
struct FunctionCaller {
    std::function<Variant(const Variant**, GDExtensionInt, GDExtensionCallError&)> functor;

    template <std::size_t... Indices>
    inline static auto to_tuple(const Variant** v, std::index_sequence<Indices...>) {
        return std::make_tuple(*v[Indices]...);
    }
    template <std::size_t N>
    inline static auto to_tuple(const Variant** v) {
        return to_tuple(v, std::make_index_sequence<N>());
    }

    FunctionCaller() : functor([](const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) -> Variant {
        return Variant(); 
    }) {}

    /* Returning void */
    FunctionCaller(const VOID_FUN0& fun) : FUNCTOR(0) {}
    FunctionCaller(const VOID_FUN1& fun) : FUNCTOR(1) {}
    FunctionCaller(const VOID_FUN2& fun) : FUNCTOR(2) {}
    FunctionCaller(const VOID_FUN3& fun) : FUNCTOR(3) {}
    FunctionCaller(const VOID_FUN4& fun) : FUNCTOR(4) {}
    FunctionCaller(const VOID_FUN5& fun) : FUNCTOR(5) {}
    FunctionCaller(const VOID_FUN6& fun) : FUNCTOR(6) {}
    FunctionCaller(const VOID_FUN7& fun) : FUNCTOR(7) {}
    FunctionCaller(const VOID_FUN8& fun) : FUNCTOR(8) {}
    FunctionCaller(const VOID_FUN9& fun) : FUNCTOR(9) {}
    FunctionCaller(const VOID_FUN10& fun) : FUNCTOR(10) {}
    FunctionCaller(const VOID_FUN11& fun) : FUNCTOR(11) {}
    FunctionCaller(const VOID_FUN12& fun) : FUNCTOR(12) {}
    FunctionCaller(const VOID_FUN13& fun) : FUNCTOR(13) {}
    FunctionCaller(const VOID_FUN14& fun) : FUNCTOR(14) {}
    FunctionCaller(const VOID_FUN15& fun) : FUNCTOR(15) {}

    /* Returning Variant */
    FunctionCaller(const VARIANT_FUN0& fun) : RET_FUNCTOR(0) {}
    FunctionCaller(const VARIANT_FUN1& fun) : RET_FUNCTOR(1) {}
    FunctionCaller(const VARIANT_FUN2& fun) : RET_FUNCTOR(2) {}
    FunctionCaller(const VARIANT_FUN3& fun) : RET_FUNCTOR(3) {}
    FunctionCaller(const VARIANT_FUN4& fun) : RET_FUNCTOR(4) {}
    FunctionCaller(const VARIANT_FUN5& fun) : RET_FUNCTOR(5) {}
    FunctionCaller(const VARIANT_FUN6& fun) : RET_FUNCTOR(6) {}
    FunctionCaller(const VARIANT_FUN7& fun) : RET_FUNCTOR(7) {}
    FunctionCaller(const VARIANT_FUN8& fun) : RET_FUNCTOR(8) {}
    FunctionCaller(const VARIANT_FUN9& fun) : RET_FUNCTOR(9) {}
    FunctionCaller(const VARIANT_FUN10& fun) : RET_FUNCTOR(10) {}
    FunctionCaller(const VARIANT_FUN11& fun) : RET_FUNCTOR(11) {}
    FunctionCaller(const VARIANT_FUN12& fun) : RET_FUNCTOR(12) {}
    FunctionCaller(const VARIANT_FUN13& fun) : RET_FUNCTOR(13) {}
    FunctionCaller(const VARIANT_FUN14& fun) : RET_FUNCTOR(14) {}
    FunctionCaller(const VARIANT_FUN15& fun) : RET_FUNCTOR(15) {}

    template<typename RetT, typename... Args>
    FunctionCaller(const std::function<RetT(Args...)>& fun) {
        assert(false && "RxLambda only supports lambdas with Variant argument counts below 16 and return types 'void' or 'Variant'!");
    }

    inline Variant operator()(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) const {
        return this->functor(args, arg_count, error);
    }
};

private:
    FunctionCaller _caller;

public:
    RxLambda(){}
    template<typename RetT, typename... Args>
    RxLambda(const std::function<RetT(Args...)>& fun) : _caller(fun) {}
    ~RxLambda(){}

protected:
	static void _bind_methods();

public:
    Variant _call(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);
    
    template<typename RetT, typename... Args>
    inline static Callable Lambda(const std::function<RetT(Args...)>& fun) {
        Ref<RxLambda> lambda = memnew(RxLambda(fun));
        Callable cb = Callable(*lambda, "_call");
        return cb.bindv(Array::make(lambda));
    }

    // static void Test();

};

template<typename RetT, typename... Args>
inline static Callable Lambda(const std::function<RetT(Args...)>& fun) {
    return RxLambda::Lambda(fun);
}

#endif // RX_LAMBDA_H