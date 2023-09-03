#ifndef RX_OBSERVABLE_OPERATORS_MAP_H
#define RX_OBSERVABLE_OPERATORS_MAP_H

#include "observable/observable.h"

#include "observable/operators/_zip.h"

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

static observable_op_t map_indexed_(const mapper_indexed_t<Variant, Variant>& mapper = nullptr) {
    auto _mapper = mapper ? mapper : basic::identity<const Variant&, uint64_t>;

    return pipe::compose(
        zip_with_iterable_(RX_ITERABLE(rx::infinite_iterable)),
        map_([_mapper](const Array& tup) -> Variant { return _mapper(tup[0], tup[1]); }  )
    );
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_MAP_H
