#ifndef RX_OBSERVABLE_OPERATORS_TIMESTAMP_H
#define RX_OBSERVABLE_OPERATORS_TIMESTAMP_H

#include "observable/observable.h"
#include "observable/operators/_map.h"
#include "observable/defer.h"
#include "internal/time.h"

#include "scheduler/scenetreetimeoutscheduler.h"

using namespace rx::scheduler;
using namespace std::chrono;

namespace rx::observable::op {

static observable_op_t timestamp_(const scheduler_t& scheduler = nullptr) {

    observable_op_t timestamp = OP(source) {
        auto _scheduler = scheduler ? scheduler : SceneTreeTimeoutScheduler::singleton();

        observable_factory_t factory = [=](const scheduler_t& scheduler_) {

            auto mapper = [=](const Variant& value) -> Variant {
                Ref<AbsoluteTime> t = memnew(AbsoluteTime(scheduler_->now()));
                return Array::make(value, t);
            };
            return source->pipe(map_(mapper));
        };

        return defer_(factory);
    };

    return timestamp;
}

} // END namespace rx::observable::op

#endif // RX_OBSERVABLE_OPERATORS_TIMESTAMP_H
