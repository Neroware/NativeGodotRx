#ifndef RX_OBSERVABLE_OBSRVABLE_H
#define RX_OBSERVABLE_OBSRVABLE_H

#include "pipe.h"
#include "basic.h"

#include "abstract/observable.h"
#include "abstract/disposable.h"
#include "abstract/scheduler.h"
#include "abstract/observer.h"

#include "internal/rlock.h"

#include "disposable/disposable.h"

namespace rx::observable {

class Observable : public ObservableBase, std::enable_shared_from_this<Observable> {

public:
    RLock lock;
private:
    subscription_t _subscribe;

protected:
    Observable(const subscription_t& subscribe) 
        : _subscribe(subscribe) {}
public:
    inline static std::shared_ptr<Observable> get(const subscription_t& subscribe_) { return std::shared_ptr<Observable>(new Observable(subscribe_)); }
    inline std::shared_ptr<Observable> getptr() { return shared_from_this(); }
    ~Observable() {}

    std::shared_ptr<DisposableBase> _subscribe_core(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    );
    std::shared_ptr<DisposableBase> subscribe(
        const on_next_t& on_next = DEFAULT_ON_NEXT,
        const on_error_t& on_error = DEFAULT_ON_ERROR,
        const on_completed_t& on_completed = DEFAULT_ON_COMPLETED,
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;
    std::shared_ptr<DisposableBase> subscribe(
        const std::shared_ptr<ObserverBase>& observer, 
        const std::shared_ptr<SchedulerBase>& scheduler = nullptr
    ) override;
    
    template <typename... Args>
    inline auto pipe(Args... fns) {
        return rx::pipe::pipe(getptr(), fns...);
    }

public:
    // OBSERVABLE_CONSTRUCTORS

};

} // END namespace ry::observable

#endif // RX_OBSERVABLE_OBSRVABLE_H