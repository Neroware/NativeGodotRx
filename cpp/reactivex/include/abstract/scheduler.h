#ifndef RX_ABSTRACT_SCHEDULER_H
#define RX_ABSTRACT_SCHEDULER_H

#include "abstract/disposable.h"
#include "exception/exception.h"

#include "internal/time.h"

#include <memory>

using namespace rx::exception;

namespace rx {

namespace abstract {
    class SchedulerBase;
} // END namespace abstract

typedef std::function<std::shared_ptr<rx::abstract::DisposableBase>(const std::shared_ptr<rx::abstract::SchedulerBase>&, const Variant&)> action_t;

namespace abstract {

class SchedulerBase {
public:
    virtual time_point_t now() = 0;
    virtual std::shared_ptr<DisposableBase> schedule(const action_t& action, const Variant& state = Variant()) = 0;
    virtual std::shared_ptr<DisposableBase> schedule_absolute(const time_point_t& duetime, const action_t& action, const Variant& state = Variant()) = 0;
    virtual std::shared_ptr<DisposableBase> schedule_relative(const time_delta_t& duetime, const action_t& action, const Variant& state = Variant()) = 0;
    virtual std::shared_ptr<DisposableBase> invoke_action(const action_t& action, const Variant& state = Variant()) = 0;

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

} // END namespace abstract

} // END namespace rx

#endif // RX_ABSTRACT_SCHEDULER_H