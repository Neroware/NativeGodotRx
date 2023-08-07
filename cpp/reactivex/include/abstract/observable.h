#ifndef RX_ABSTRACT_OBSERVABLE_H
#define RX_ABSTRACT_OBSERVABLE_H

#include "abstract/disposable.h"
#include "abstract/observer.h"
#include "abstract/scheduler.h"

#include "basic.h"

#include <memory>

namespace rx {
namespace abstract {

class ObservableBase {

public:
    virtual std::shared_ptr<DisposableBase> subscribe(const on_next_t& on_next, const on_error_t& on_error, const on_completed_t& on_completed, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) = 0;
    virtual std::shared_ptr<DisposableBase> subscribe(const std::shared_ptr<ObserverBase>& observer, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) = 0;

};

} // END namespace abstract

typedef std::function<std::shared_ptr<rx::abstract::DisposableBase>(const std::shared_ptr<rx::abstract::ObserverBase>&, const std::shared_ptr<rx::abstract::SchedulerBase>&)> subscription_t;

} // END namespace rx

#endif // RX_ABSTRACT_OBSERVABLE_H