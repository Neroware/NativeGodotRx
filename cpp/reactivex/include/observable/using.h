#ifndef RX_OBSERVABLE_USING_H
#define RX_OBSERVABLE_USING_H

#include "observable/observable.h"
#include "observable/throw.h"

#include "internal/utils.h"
#include "disposable/compositedisposable.h"

using namespace rx::disposable;

namespace rx::observable {

static std::shared_ptr<Observable> using_(
    const std::function<disposable_t()>& resource_factory,
    const std::function<std::shared_ptr<Observable>(const disposable_t&)>& observable_factory
) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        disposable_t disp = std::make_shared<Disposable>();

        disposable_t resource;
        std::shared_ptr<Observable> source;
        try {
            resource = resource_factory();
            if (resource) {
                disp = resource;
            }

            source = observable_factory(resource);
        }
        catch(...) {
            auto d = throw_(std::current_exception())->subscribe(observer, scheduler);
            return std::make_shared<CompositeDisposable>(disposable_list_t{d, disp});
        }

        return std::make_shared<CompositeDisposable>(disposable_list_t{
            source->subscribe(observer, scheduler), disp
        });

    };

    return Observable::get(subscribe);
}

}

#endif // RX_OBSERVABLE_USING_H