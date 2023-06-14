#include "lambda.h"

#include <godot_cpp/variant/utility_functions.hpp>

void RxLambda::_bind_methods() {
    {
		MethodInfo mi;
		mi.arguments.push_back(PropertyInfo(Variant::NIL, "data"));
		mi.name = "_call";
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "_call", &RxLambda::_call, mi);
	}
    // ClassDB::bind_static_method("RxLambda", D_METHOD("Test"), &RxLambda::Test);
}

Variant RxLambda::_call(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error) {
    return this->_caller(args, arg_count, error);
}

template<typename RetT, typename... Args>
Callable RxLambda::Lambda(std::function<RetT(Args...)> fun) {
    return Callable(memnew(RxLambda(fun)), "_call");
}

/* void RxLambda::Test() {
    UtilityFunctions::print("Testing lambdas...");
    auto cb = RxLambda::Lambda(VARIANT_FUN3([](int a, int b, const String& s) {
        UtilityFunctions::print("a> ", a);
        UtilityFunctions::print("b> ", b);
        UtilityFunctions::print("s> ", s);
        return a + b;
    }));
    UtilityFunctions::print("Lambda Test: ", cb.callv(Array::make(40, 2, "foo")));
} */