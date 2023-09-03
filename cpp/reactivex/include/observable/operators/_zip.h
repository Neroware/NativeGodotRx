#ifndef RX_OBSERVABLE_OPERATORS_ZIP_H
#define RX_OBSERVABLE_OPERATORS_ZIP_H

#include "observable/observable.h"
#include "observable/zip.h"

#include "internal/utils.h"

#include "typing.h"

namespace rx::observable::op {

template<typename IterableT>
static observable_op_t zip_(const IterableT& args) {
    observable_vec_t sources;
    sources.push_back(nullptr);
    for (auto& obs : args) sources.push_back(obs);

    observable_op_t zip = OP(source) {
        
        observable_vec_t _sources(sources);
        _sources[0] = source;

        return rx::observable::zip_(_sources); 
    };

    return zip;
}
template <typename... Args>
static observable_op_t zip_(const Args&... others) {

    observable_op_t zip = OP(source) {
        return rx::observable::zip_(source, others...);
    };

    return zip;
}

template<typename IterableT>
static observable_op_t zip_with_iterable_(const IterableT& seq) {
    observable_op_t zip_with_iterable = OP(source) {

        auto first = source;
        auto second =  std::make_shared<typename IterableT::const_iterator>(seq.begin());

        subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
            on_next_t on_next = [=](const Variant& left) {
                try {
                    if (!(*second != seq.end())) {
                        observer->on_completed();
                    } else {
                        const Variant right = **second;
                        Array result = Array::make(left, right);
                        observer->on_next(result);
                        ++(*second);
                    }
                } catch (...) {
                    observer->on_error(std::current_exception());
                }
            };

            return first->subscribe(
                on_next, ONERROR_FWD(observer), ONCOMPLETED_FWD(observer), scheduler
            );
        };

        return Observable::get(subscribe);
    };

    return zip_with_iterable;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_ZIP_H
