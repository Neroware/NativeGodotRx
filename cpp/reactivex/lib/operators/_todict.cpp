#include "observable/definitions.h"

namespace rx::observable::op {

observable_op_t ops::to_dict_(
    const mapper_t<Variant, Variant>& key_mapper,
    const mapper_t<Variant, Variant>& element_mapper
) {

    observable_op_t to_dict = OP(source) {

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            auto m = std::make_shared<Dictionary>();

            auto on_next = [=](const Variant& x) {
                Variant key;
                try {
                    key = key_mapper(x);
                }
                catch(...) {
                    observer->on_error(std::current_exception());
                    return;
                }

                Variant element;
                if (element_mapper) {
                    try {
                        element = element_mapper(x);
                    }
                    catch(...) {
                        observer->on_error(std::current_exception());
                        return;
                    }
                }
                else {
                    element = x;
                }

                (*m)[key] = element;
            };

            auto on_completed = [=]() {
                observer->on_next(*m);
                *m = Dictionary();
                observer->on_completed();
            };

            return source->subscribe(
                on_next, ONERROR_FWD(observer), 
                on_completed, scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return to_dict;
}

} // END namespace rx::observable::op
