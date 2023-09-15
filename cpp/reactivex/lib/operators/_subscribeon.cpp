#include "observable/definitions.h"

#include "disposable/singleassignmentdisposable.h"
#include "disposable/serialdisposable.h"
#include "disposable/scheduleddisposable.h"


namespace rx::observable::op {

observable_op_t subscribe_on_(const rx_observable_t& source, const scheduler_t& scheduler) {

    auto subscribe_on = OP(source) {

        auto subscribe = SUBSCRIBE(observer, __ = nullptr) {
            auto m = std::make_shared<SingleAssignmentDisposable>();
            auto d = std::make_shared<SerialDisposable>();
            d->set_disposable(m);

            auto action = ACTION(scheduler, state) {
                d->set_disposable(ScheduledDisposable::get(scheduler, source->subscribe(observer)));
                return nullptr;
            };

            m->set_disposable(scheduler->schedule(action));
            return d;
        };

        return Observable::get(subscribe);

    };

    return subscribe_on;
}

}