#ifndef RX_OBSERVABLE_OPERATORS_MAP_H
#define RX_OBSERVABLE_OPERATORS_MAP_H

#include "observable/observable.h"

#include "internal/utils.h"
#include "typing.h"

using namespace rx::disposable;

namespace rx::observable::op {

static observable_op_t map_(const mapper_t<Variant, Variant>& mapper = nullptr) {

    auto _mapper = mapper ? mapper : basic::identity<const Variant&>;

    observable_op_t map = OP(source) {
        subscription_t subscribe = SUBSCRIBE(obv, scheduler = nullptr) {
            auto on_next = [=](const Variant& value) {
                try {
                    Variant result = _mapper(value);
                    obv->on_next(result);
                } catch (...) {
                    obv->on_error(std::current_exception());
                }
            };

            return source->subscribe(
                on_next, ONERROR_FWD(obv), 
                ONCOMPLETED_FWD(obv), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return map;
}

// TODO map_indexed

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MAP_H
