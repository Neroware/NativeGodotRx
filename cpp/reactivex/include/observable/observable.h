#ifndef RX_OBSERVABLE_OBSRVABLE_H
#define RX_OBSERVABLE_OBSRVABLE_H

#include "pipe.h"
#include "basic.h"
#include "godotrx.h"

#include "abstract/observable.h"
#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "abstract/observer.h"
#include "abstract/iterable.h"

#include "internal/rlock.h"
#include "internal/virtualsharedfromthis.h"

#include "disposable/disposable.h"

#include "internal/utils.h"

using namespace rx::disposable;

namespace rx::observable {

class Observable : public ObservableBase, public std::virtual_enable_shared_from_this<Observable> {

public:
    RLock lock;
private:
    subscription_t _subscribe;

protected:
    Observable(const subscription_t& subscribe = DEFAULT_SUBSCRIBE) 
        : _subscribe(subscribe) {}
public:
    inline static rx_observable_t get(const subscription_t& subscribe_) { return std::shared_ptr<Observable>(new Observable(subscribe_)); }
    inline rx_observable_t getptr() { return shared_from_this(); }
    ~Observable() {}

    disposable_t subscribe(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED,
        const scheduler_t& scheduler = nullptr
    ) override;
    disposable_t subscribe(
        const observer_t& observer, 
        const scheduler_t& scheduler = nullptr
    ) override;
    
    template <typename... Ops>
    inline auto pipe(Ops... fns) {
        return rx::pipe::pipe(getptr(), fns...);
    }

private:
    virtual disposable_t _subscribe_core(
        const observer_t& observer, 
        const scheduler_t& scheduler = nullptr
    );

};
} // END namespace rx::observable

#endif // RX_OBSERVABLE_OBSRVABLE_H