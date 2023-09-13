#include "observable/definitions.h"

#include "internal/time.h"

#include "scheduler/scenetreetimeoutscheduler.h"

using namespace rx::scheduler;

namespace rx::observable::op {

observable_op_t ops::timestamp_(const scheduler_t& scheduler) {

    observable_op_t timestamp = OP(source) {
        auto _scheduler = scheduler ? scheduler : SceneTreeTimeoutScheduler::singleton();

        observable_factory_t factory = [=](const scheduler_t& scheduler_) {

            auto mapper = [=](const Variant& value) -> Variant {
                Ref<AbsoluteTime> t = memnew(AbsoluteTime(scheduler_->now()));
                return RxTimeStamp::from_abs(t, value);
            };
            return source->pipe(map(mapper));
        };

        return defer(factory);
    };

    return timestamp;
}

} // END namespace rx::observable::op
