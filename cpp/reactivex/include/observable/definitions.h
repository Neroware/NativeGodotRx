#ifndef RX_OBSERVABLE_DEFINITIONS_H
#define RX_OBSERVABLE_DEFINITIONS_H

#include "observable/observable.h"

#include "internal/mapping.h"

namespace rx {

namespace observable {

struct obs {

    // case.h
    static rx_observable_t case_(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr);
    // defer.h
    static rx_observable_t defer_(const observable_factory_t& factory);
    // empty.h
    static rx_observable_t empty_(const scheduler_t& scheduler = nullptr);
    // throw.h
    static rx_observable_t throw_(const error_t& error, const scheduler_t& scheduler = nullptr);

}; // END struct obs

// case.h
static rx_observable_t case_mapper(const mapper_t<Variant>& mapper, const default_mapping<Variant, rx_observable_t>& sources, const rx_observable_t& default_source = nullptr) { return obs::case_(mapper, sources, default_source); }
// defer.h
static rx_observable_t defer(const observable_factory_t& factory) { return obs::defer_(factory); }
// empty.h
static rx_observable_t empty(const scheduler_t& scheduler = nullptr) { return obs::empty_(scheduler); }
// throw.h
static rx_observable_t throw_error(const error_t& error, const scheduler_t& scheduler = nullptr) { return obs::throw_(error); }

} // END namespace observable

namespace op {

struct operators {


}; // END struct operators

} // END namespace op

} // END namespace rx::observable

#endif // RX_OBSERVABLE_DEFINITIONS_H