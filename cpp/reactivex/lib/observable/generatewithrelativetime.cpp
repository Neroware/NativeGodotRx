#include "observable/definitions.h"

#include "disposable/multiassignmentdisposable.h"
#include "scheduler/scenetreetimeoutscheduler.h"

namespace rx::observable {

rx_observable_t obs::generate_with_relative_time_(
    const Variant& initial_state,
    const predicate_t<Variant>& condition,
    const mapper_t<Variant, Variant>& iterate,
    const mapper_t<time_delta_t, Variant>& time_mapper
) {
    auto subscribe = SUBSCRIBE(observer, scheduler = nullptr) {
        scheduler_t _scheduler = scheduler ? scheduler : SceneTreeTimeoutScheduler::singleton();
        auto mad = std::make_shared<MultiAssignmentDisposable>();

        auto state = std::make_shared<Variant>();
        auto has_result = std::make_shared<bool>(false);
        auto result = std::make_shared<Variant>();
        auto first = std::make_shared<bool>(true);
        auto time = std::make_shared<time_delta_t>();

        auto action = RECURSIVE_ACTION(scheduler, __, _action) {
            if (*has_result) {
                observer->on_next(*result);
            }

            try {
                if (*first) {
                    *first = false;
                } else {
                    *state = iterate(*state);
                }

                *has_result = condition(*state);

                if (*has_result) {
                    *result = *state;
                    *time = time_mapper(*state);
                }
            } catch (...) {
                observer->on_error(std::current_exception());
                return nullptr;
            }

            if (*has_result) {
                mad->set_disposable(scheduler->schedule_relative(*time, RECURSIVE_ACTION_FWD(_action)));
            } else {
                observer->on_completed();
            }
            return nullptr;
        };

        mad->set_disposable(_scheduler->schedule_relative(DELTA_ZERO, RECURSIVE_ACTION_FWD(action)));
        return mad;
    };

    return Observable::get(subscribe);
}

} // END namespace rx::observable
