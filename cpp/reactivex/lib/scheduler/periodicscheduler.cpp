#include "scheduler/periodicscheduler.h"

#include "disposable/multiassignmentdisposable.h"
#include "internal/utils.h"

using namespace rx::disposable;

namespace rx::scheduler {

disposable_t PeriodicScheduler::schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state) {
    auto disp = std::make_shared<MultiAssignmentDisposable>();
    time_delta_t seconds = period;

    auto periodic = RECURSIVE_ACTION(scheduler, state, _periodic) {
        if (disp->is_disposed) {
            return nullptr;
        }

        auto now = scheduler->now();

        Variant _state = state;
        try {
            _state = action(state);
        }
        catch(...) {
            disp->dispose();
            throw;
        }

        time_delta_t time = seconds - (scheduler->now() - now);
        disp->set_disposable(
            scheduler->schedule_relative(time, RECURSIVE_ACTION_FWD(_periodic), _state)
        );

        return nullptr;
    };

    disp->set_disposable(
        this->schedule_relative(period, RECURSIVE_ACTION_FWD(periodic), state)
    );
    return disp;
}

} // END namespace rx::scheduler