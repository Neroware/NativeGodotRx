#ifndef RX_OBSERVABLE_USING_H
#define RX_OBSERVABLE_USING_H

#include "observable/definitions.h"

#include "internal/utils.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

rx_observable_t obs::using_(
    const std::function<disposable_t()>& resource_factory,
    const std::function<rx_observable_t(const disposable_t&)>& observable_factory
) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        disposable_t disp = std::make_shared<Disposable>();

        disposable_t resource;
        rx_observable_t source;
        try {
            resource = resource_factory();
            if (resource) {
                disp = resource;
            }

            source = observable_factory(resource);
        }
        catch(...) {
            auto d = throw_error(std::current_exception())->subscribe(observer, scheduler);
            return std::make_shared<CompositeDisposable>(d, disp);
        }

        return std::make_shared<CompositeDisposable>(
            source->subscribe(observer, scheduler), disp
        );

    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_USING_H