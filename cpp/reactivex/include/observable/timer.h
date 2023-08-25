#ifndef RX_OBSERVABLE_TIMER_H
#define RX_OBSERVABLE_TIMER_H

#include "observable/observable.h"

#include "disposable/multiassignmentdisposable.h"

using namespace rx::scheduler;
using namespace rx::disposable;

namespace rx::observable {

inline static time_point_t _get_duetime(const time_point_t& t, const std::shared_ptr<SchedulerBase>& s) {
    return t;
}
inline static time_point_t _get_duetime(const time_delta_t& t, const std::shared_ptr<SchedulerBase>& s) {
    return s->now() + t;
}
inline static time_point_t _get_duetime(const double t, const std::shared_ptr<SchedulerBase>& s) {
    return s->now() + Scheduler::to_timedelta(t);
}

static std::shared_ptr<Observable> observable_timer_date(const time_point_t& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();

        action_t action = ACTION(scheduler__, state) {
            observer->on_next(0);
            observer->on_completed();
            return nullptr;
        };

        return _scheduler->schedule_absolute(duetime, action);
    };

    return Observable::get(subscribe);
}

template<typename timeT0, typename timeT1>
static std::shared_ptr<Observable> observable_timer_duetime_and_period(const timeT0& duetime_, const timeT1& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {

    auto duetime = std::make_shared<time_point_t>();
    auto duetime_valid = std::make_shared<bool>(false);

    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();

        if (!*duetime_valid) {
            *duetime = _get_duetime(duetime_, _scheduler);
            *duetime_valid = true;
        }

        double p = std::max(0.0, Scheduler::to_seconds(period));
        auto mad = std::make_shared<MultiAssignmentDisposable>();
        auto dt = std::make_shared<time_point_t>(*duetime);
        auto count = std::make_shared<int>(0);

        auto action = RECURSIVE_ACTION(scheduler__, state, _action) {
            if (p > 0.0) {
                time_point_t now = scheduler__->now();
                *dt = *dt + Scheduler::to_timedelta(p);
                if (*dt <= now) {
                    *dt = now + Scheduler::to_timedelta(p);
                }
            }

            observer->on_next((*count)++);
            mad->set_disposable(scheduler__->schedule_absolute(*dt, RECURSIVE_ACTION_FWD(_action)));
            return nullptr;
        };

        mad->set_disposable(_scheduler->schedule_absolute(*dt, RECURSIVE_ACTION_FWD(action)));
        return mad;
    };

    return Observable::get(subscribe);
}

static std::shared_ptr<Observable> observable_timer_timespan(const time_delta_t& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    subscription_t subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
        auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();
        double d = Scheduler::to_seconds(duetime);

        action_t action = ACTION(scheduler__, state) {
            observer->on_next(0);
            observer->on_completed();
            return nullptr;
        };

        if (d <= 0.0) {
            return _scheduler->schedule(action);
        }
        return _scheduler->schedule_relative(duetime, action);
    };

    return Observable::get(subscribe);
}

static std::shared_ptr<Observable> observable_timer_timespan_and_period(const time_delta_t& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {

    if (duetime == period) {

        auto subscribe = SUBSCRIBE(observer, scheduler_ = nullptr) {
            auto _scheduler = scheduler ? scheduler : scheduler_ ? scheduler_ : SceneTreeTimeoutScheduler::singleton();

            periodic_action_t action = PERIODIC_ACTION(count) {
                if (count.get_type() != Variant::NIL) {
                    uint64_t _count = count;
                    observer->on_next(_count);
                    return _count + 1;
                }
                return Variant();
            };

            if (auto periodic_scheduler = std::dynamic_pointer_cast<PeriodicScheduler>(_scheduler)) {
                return periodic_scheduler->schedule_periodic(period, action, 0);
            }
            
            throw BadArgumentException("Sceduler must be PeriodicScheduler");
        };

        return Observable::get(subscribe);

    }

    return observable_timer_duetime_and_period(duetime, period, scheduler);
}

inline static std::shared_ptr<Observable> timer_(const time_point_t& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_date(duetime, scheduler);
}
inline static std::shared_ptr<Observable> timer_(const time_point_t& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_duetime_and_period(duetime, period, scheduler);
}
inline static std::shared_ptr<Observable> timer_(const time_delta_t& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_timespan(duetime, scheduler);
}
inline static std::shared_ptr<Observable> timer_(const time_delta_t& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_timespan_and_period(duetime, period, scheduler);
}
inline static std::shared_ptr<Observable> timer_(const double& duetime, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_timespan(Scheduler::to_timedelta(duetime), scheduler);
}
inline static std::shared_ptr<Observable> timer_(const double& duetime, const time_delta_t& period, const std::shared_ptr<SchedulerBase>& scheduler = nullptr) {
    return observable_timer_timespan_and_period(Scheduler::to_timedelta(duetime), period, scheduler);
}

} // END namespace rx::observable

#endif // RX_OBSERVABLE_TIMER_H