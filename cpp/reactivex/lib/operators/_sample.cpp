#include "observable/definitions.h"

#include "disposable/compositedisposable.h"

namespace rx::observable::op {

rx_observable_t ops::sample_observable(const rx_observable_t& source, const rx_observable_t& sampler) {

    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {

        auto at_end = std::make_shared<bool>(false);
        auto has_value = std::make_shared<bool>(false);
        auto value = std::make_shared<Variant>();

        auto sample_subscribe = [=](const Variant& = VNULL) {
            if (*has_value) {
                *has_value = false;
                observer->on_next(*value);
            }

            if (*at_end) {
                observer->on_completed();
            }
        };

        on_next_t on_next = [=](const Variant& new_value) {
            *has_value = true;
            *value = new_value;
        };

        on_completed_t on_completed = [=]() {
            *at_end = true;
        };

        return std::make_shared<CompositeDisposable>(
            source->subscribe(
                on_next, ONERROR_FWD(observer), on_completed, scheduler
            ),
            sampler->subscribe(
                sample_subscribe, ONERROR_FWD(observer), sample_subscribe, scheduler
            )
        );
    };

    return Observable::get(subscribe);
}

observable_op_t ops::sample_(const time_delta_t& sampler, const scheduler_t& scheduler) {
    auto sample = OP(source) {
        return sample_observable(source, interval(sampler, scheduler));
    };
    return sample;
}
observable_op_t ops::sample_(const rx_observable_t& sampler) {
    auto sample = OP(source) {
        return sample_observable(source, sampler);
    };
    return sample;
}


} // END namespace rx::observable::op
