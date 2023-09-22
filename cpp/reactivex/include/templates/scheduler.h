#ifndef RX_TEMPLATES_SCHEDULER_H
#define RX_TEMPLATES_SCHEDULER_H

#include "wrapper/templates.h"

#include "scheduler/scheduler.h"

using namespace rx::wrappers;
using namespace rx::scheduler;

namespace rx::templates {

class Scheduler_ : public Scheduler {

private:
    Ref<RxSchedulerTemplate_> self;
protected:
    Scheduler_(){}
public:
    ~Scheduler_(){}
    inline scheduler_ getptr() { return std::static_pointer_cast<Scheduler_>(Scheduler::getptr()); }
    inline static scheduler_ get() { return std::shared_ptr<Scheduler_>(new Scheduler_()); }
    inline void _template(Ref<RxSchedulerTemplate_> t) { this->self = t; }

    inline time_point_t now() override {
        if (_has_template_method(self, "_now"))
            return Ref<AbsoluteTime>(RX_TEMPLATE_CALL(_now, self, getptr()))->t;
        return Scheduler::now();
    }
    inline disposable_t invoke_action(const scheduled_action_t& action, const Variant& state = Variant()) override {
        if (_has_template_method(self, "_invoke_action"))
            return RX_TEMPLATE_CALL(_invoke_action, self, getptr(), memnew(RxAction(action)), state);
        return Scheduler::invoke_action(action, state);
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
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Scheduler>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

class PeriodicScheduler_ : public PeriodicScheduler {

private:
    Ref<RxPeriodicSchedulerTemplate_> self;
protected:
    PeriodicScheduler_(){}
public:
    ~PeriodicScheduler_(){}
    inline periodic_scheduler_ getptr() { return std::static_pointer_cast<PeriodicScheduler_>(PeriodicScheduler::getptr()); }
    inline static periodic_scheduler_ get() { return std::shared_ptr<PeriodicScheduler_>(new PeriodicScheduler_()); }
    inline void _template(Ref<RxPeriodicSchedulerTemplate_> t) { this->self = t; }

    inline time_point_t now() override {
        if (_has_template_method(self, "_now"))
            return Ref<AbsoluteTime>(RX_TEMPLATE_CALL(_now, self, getptr()))->t;
        return Scheduler::now();
    }
    inline disposable_t invoke_action(const scheduled_action_t& action, const Variant& state = Variant()) override {
        if (_has_template_method(self, "_invoke_action"))
            return RX_TEMPLATE_CALL(_invoke_action, self, getptr(), memnew(RxAction(action)), state);
        return Scheduler::invoke_action(action, state);
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
    inline disposable_t schedule_periodic(const time_delta_t& period, const periodic_action_t& action, const Variant& state = Variant()) override {
        return RX_TEMPLATE_CALL(_schedule_periodic, self, getptr(), memnew(RelativeTime(period)), memnew(RxPeriodicAction(action)), state);
    }
    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Scheduler>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

class GodotSignalScheduler_ : public GodotSignalScheduler {

private:
    Ref<RxGodotSignalSchedulerTemplate_> self;
protected:
    GodotSignalScheduler_(){}
public:
    ~GodotSignalScheduler_(){}
    inline gd_signal_scheduler_ getptr() { return std::static_pointer_cast<GodotSignalScheduler_>(GodotSignalScheduler::getptr()); }
    inline static gd_signal_scheduler_ get() { return std::shared_ptr<GodotSignalScheduler_>(new GodotSignalScheduler_()); }
    inline void _template(Ref<RxGodotSignalSchedulerTemplate_> t) { this->self = t; }

    inline time_point_t now() override {
        if (_has_template_method(self, "_now"))
            return Ref<AbsoluteTime>(RX_TEMPLATE_CALL(_now, self, getptr()))->t;
        return Scheduler::now();
    }
    inline disposable_t invoke_action(const scheduled_action_t& action, const Variant& state = Variant()) override {
        if (_has_template_method(self, "_invoke_action"))
            return RX_TEMPLATE_CALL(_invoke_action, self, getptr(), memnew(RxAction(action)), state);
        return Scheduler::invoke_action(action, state);
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
    inline disposable_t schedule_signal(const Object* owner, const StringName& signal_name, const periodic_action_t& action, const Variant& state = Variant()) override {
        if (_has_template_method(self, "_schedule_signal"))
            return RX_TEMPLATE_CALL(_schedule_signal, self, getptr(), owner, signal_name, memnew(RxPeriodicAction(action)), state);
        return GodotSignalScheduler::schedule_signal(owner, signal_name, action, state);
    }
    inline StringName classname() {
        if (!_has_template_method(self, "_classname"))
            return "<<Anonymous Scheduler>>";
        return RX_TEMPLATE_CALL(_classname, self);
    } 
};

} // END namespace rx::templates

#endif // RX_TEMPLATES_SCHEDULER_H