#ifndef RX_OBSERVABLE_IFTHEN_H
#define RX_OBSERVABLE_IFTHEN_H

#include "observable/observable.h"
#include "observable/empty.h"
#include "observable/defer.h"

#include "typing.h"

using namespace rx::disposable;

namespace rx::observable {

static std::shared_ptr<Observable> if_then_(
    const predicate_t<>& condition,
    const std::shared_ptr<Observable>& then_source,
    const std::shared_ptr<Observable>& else_source = nullptr
) {

    auto else_source_ = else_source ? else_source : empty_();

    observable_factory_t factory = [=](const std::shared_ptr<rx::abstract::SchedulerBase> & scheduler) -> std::shared_ptr<rx::observable::Observable> {
        return condition() ? then_source : else_source_;
    };

    return defer_(factory);
}

}

#endif // RX_OBSERVABLE_IFTHEN_H