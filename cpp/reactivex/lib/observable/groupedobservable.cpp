#include "observable/groupedobservable.h"

#include "disposable/compositedisposable.h"

namespace rx::observable {

GroupedObservable::GroupedObservable(const Variant& key_, const rx_observable_t& underlying_observable, const std::shared_ptr<RefCountDisposable>& merged_disposable) 
    : key(key_) {

    subscription_t subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        return std::make_shared<CompositeDisposable>(
            merged_disposable ? merged_disposable->get_disposable() : std::make_shared<Disposable>(),
            underlying_observable->subscribe(observer, scheduler)
        );
    };

    this->underlying_observable = !merged_disposable ? underlying_observable : Observable::get(subscribe);

}

disposable_t GroupedObservable::_subscribe_core(const observer_t& observer, const scheduler_t& scheduler) {
    return this->underlying_observable->subscribe(observer, scheduler);
}

} // END namespace rx::observable