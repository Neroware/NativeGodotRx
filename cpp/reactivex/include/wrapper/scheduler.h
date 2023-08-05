#ifndef RX_WRAPPER_SCHEDULER_H
#define RX_WRAPPER_SCHEDULER_H

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

#include "abstract/scheduler.h"
#include "exception/exception.h"
#include "wrapper/disposable.h"
#include "cast.h"

using namespace godot;
using namespace rx::abstract;
using namespace rx::exception;

namespace rx {
namespace wrappers {

class RxScheduler : public RefCounted {
    GDCLASS(RxScheduler, RefCounted)

private:
    std::shared_ptr<SchedulerBase> _ptr;

public:
    RxScheduler() { throw NotImplementedException(); }
    RxScheduler(const std::shared_ptr<SchedulerBase>& scheduler) : _ptr(scheduler) {}
    ~RxScheduler(){}

protected:
    static inline void _bind_methods() {
        ClassDB::bind_static_method("RxScheduler", D_METHOD("to_seconds", "value"), &RxScheduler::to_seconds);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("to_datetime", "value"), &RxScheduler::to_datetime);
        ClassDB::bind_static_method("RxScheduler", D_METHOD("to_timedelta", "value"), &RxScheduler::to_timedelta);
        ClassDB::bind_method(D_METHOD("schedule", "action", "state"), &RxScheduler::schedule, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("schedule_absolute", "duetime", "action", "state"), &RxScheduler::schedule_absolute, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("schedule_relative", "duetime", "action", "state"), &RxScheduler::schedule_relative, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("now"), &RxScheduler::now);
        ClassDB::bind_method(D_METHOD("invoke_action", "action", "state"), &RxScheduler::invoke_action, DEFVAL(Variant()));
    }

public:
    static inline Ref<RxScheduler> wrap(const std::shared_ptr<SchedulerBase>& scheduler) {
        return memnew(RxScheduler(scheduler));
    }
    inline std::shared_ptr<SchedulerBase> unwrap() const { return this->_ptr; }

    static double to_seconds(const Variant& value);
    static Ref<AbsoluteTime> to_datetime(const Variant& value);
    static Ref<RelativeTime> to_timedelta(const Variant& value);

    Ref<RxDisposable> schedule(const Callable& action, const Variant& state = Variant());
    Ref<RxDisposable> schedule_absolute(Ref<AbsoluteTime> duetime, const Callable& action, const Variant& state = Variant());
    Ref<RxDisposable> schedule_relative(Ref<RelativeTime> duetime, const Callable& action, const Variant& state = Variant());
    Ref<AbsoluteTime> now();
    Ref<RxDisposable> invoke_action(const Callable& action, const Variant& state);

}; // END class RxScheduler

} // END namespace wrappers

static action_t action_cb(const Callable& cb) {
    return action_t([cb](const std::shared_ptr<rx::abstract::SchedulerBase>& scheduler, const Variant& state = Variant()){
        auto res = cb.callv(Array::make(rx::wrappers::RxScheduler::wrap(scheduler), state));
        if (auto casted = DYN_CAST_OR_NULL(res, rx::wrappers::RxDisposable)) {
            return casted->unwrap();
        }
        return std::shared_ptr<rx::abstract::DisposableBase>();
    });
}

} // END namespace rx
#endif // RX_WRAPPER_SCHEDULER_H