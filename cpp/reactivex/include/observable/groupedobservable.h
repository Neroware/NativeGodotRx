#ifndef RX_OBSERVABLE_GROUPEDOBSERVABLE_H
#define RX_OBSERVABLE_GROUPEDOBSERVABLE_H

#include "observable/observable.h"

#include "disposable/refcountdisposable.h"

#include "typing.h"
#include "internal/utils.h"

using namespace rx::disposable;

namespace rx::observable {

class GroupedObservable : public Observable {

public:
    Variant key;
    rx_observable_t underlying_observable;
    
protected:
    GroupedObservable(const Variant& key, const rx_observable_t& underlying_observable, const std::shared_ptr<RefCountDisposable>& merged_disposable = nullptr);

public:
    ~GroupedObservable(){}
    inline static rx_grouped_observable_t get(const Variant& key_, const rx_observable_t& underlying_observable_, const std::shared_ptr<RefCountDisposable>& merged_disposable = nullptr) { return std::shared_ptr<GroupedObservable>(new GroupedObservable(key_, underlying_observable_, merged_disposable)); }
    inline rx_grouped_observable_t getptr() { return std::static_pointer_cast<GroupedObservable>(Observable::getptr()); }

private:
    virtual disposable_t _subscribe_core(const observer_t& observer, const scheduler_t& scheduler = nullptr) override;


}; // END class GroupedObservable

} // rx::observable

#endif // RX_OBSERVABLE_GROUPEDOBSERVABLE_H