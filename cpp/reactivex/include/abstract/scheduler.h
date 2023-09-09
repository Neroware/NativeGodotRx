#ifndef RX_ABSTRACT_SCHEDULER_H
#define RX_ABSTRACT_SCHEDULER_H

#include "internal/time.h"
#include "exception/exception.h"
#include "typing.h"

using namespace rx::exception;

namespace rx::abstract {

class SchedulerBase {
public:
    virtual time_point_t now() = 0;
    virtual disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) = 0;
    virtual disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) = 0;
    virtual disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) = 0;
    virtual disposable_t invoke_action(const scheduled_action_t& action, const Variant& state = Variant()) = 0;

    template<typename timeT>
    static double to_seconds(const timeT& value) {
        throw NotImplementedException();
    }
    template<typename timeT>
    static time_delta_t to_timedelta(const timeT& value) {
        throw NotImplementedException();
    }
    template<typename timeT>
    static time_point_t to_datetime(const timeT& value) {
        throw NotImplementedException();
    }

};

} // END namespace rx::abstract

#endif // RX_ABSTRACT_SCHEDULER_H