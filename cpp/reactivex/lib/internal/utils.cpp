#include "internal/utils.h"

#include "observable/observable.h"
#include "disposable/compositedisposable.h"

#include "godotrx.h"
#include "cast.h"

using namespace rx::disposable;
using namespace rx::observable;

namespace rx {

bool NotSet::is(const Variant& v) {
    return IS_CLASS(v, NotSet);
}

Ref<NotSet> NotSet::value() {
    return GDRX->NOT_SET;
}

rx_observable_t add_ref(const rx_observable_t& xs, const std::shared_ptr<RefCountDisposable>& r) {
    subscription_t subscribe = SUBSCRIBE(observer, __ = nullptr) {
        return std::make_shared<CompositeDisposable>(
            r->get_disposable(),
            xs->subscribe(observer)
        );
    };

    return Observable::get(subscribe);
}

}