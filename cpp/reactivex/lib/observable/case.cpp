#include "observable/definitions.h"

namespace rx::observable {

rx_observable_t obs::case_(
    const mapper_t<Variant>& mapper,
    const default_mapping<Variant, rx_observable_t>& sources,
    const rx_observable_t& default_source
) {

    auto default_source_ = default_source ? default_source : empty();

    observable_factory_t factory = [=](const scheduler_t& _) -> rx_observable_t {
        Variant key = mapper();
        rx_observable_t result;
        if (sources.contains(key)) {
            result = sources[key];
        }
        else {
            result = default_source_;
        }

        return result;
    };

    return defer(factory);
}

} // END namespace rx::observable