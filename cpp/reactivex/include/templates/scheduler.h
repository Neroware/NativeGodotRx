#ifndef RX_TEMPLATES_SCHEDULER_H
#define RX_TEMPLATES_SCHEDULER_H

#include "wrapper/templates.h"

#include "scheduler/scheduler.h"

using namespace rx::wrappers;
using namespace rx::scheduler;

namespace rx::templates {

class Scheduler_ : public Scheduler {

private:
    Ref<RxSchedulerTemplate> self;
protected:
    Scheduler_(){}
public:
    ~Scheduler_(){}
    inline scheduler_ getptr() { return std::static_pointer_cast<Scheduler_>(Scheduler::getptr()); }
    inline static scheduler_ get() { return std::shared_ptr<Scheduler_>(new Scheduler_()); }
    inline void _template(Ref<RxSchedulerTemplate> t) { this->self = t; }

    inline time_point_t now() override {
        return Ref<AbsoluteTime>(RX_TEMPLATE_CALL(_now, self, getptr()))->t;
    }
    inline disposable_t invoke_action(const scheduled_action_t& action, const Variant& state = Variant()) override {
        return RX_TEMPLATE_CALL(_invoke_action, self, getptr(), memnew(RxAction(action)), state);
    }
    inline disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override {
        return RX_TEMPLATE_CALL(_schedule, self, getptr(), memnew(RxAction(action)), state);
    }
    inline disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override {
        return RX_TEMPLATE_CALL(_schedule_absolute, self, getptr(), memnew(AbsoluteTime(duetime)), memnew(RxAction(action)), state);
    }
    inline disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override {
        return RX_TEMPLATE_CALL(_schedule_relative, self, getptr(), memnew(RelativeTime(duetime)), memnew(RxAction(action)), state);
    }
    inline StringName classname() {
        return RX_TEMPLATE_CALL(_classname, self);
    } 

};

} // END namespace rx::templates

#endif // RX_TEMPLATES_SCHEDULER_H