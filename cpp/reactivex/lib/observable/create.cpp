#include "observable/definitions.h"

namespace rx::observable {

rx_observable_t obs::create_(const subscription_t& subscribe) {
    return Observable::get(subscribe ? subscribe : DEFAULT_SUBSCRIBE);
}

}