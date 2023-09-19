#ifndef RX_SCHEDULER_GODOTSIGNALSCHEDULER_H
#define RX_SCHEDULER_GODOTSIGNALSCHEDULER_H

#include "scheduler/scheduler.h"
#include "abstract/godotsignalscheduler.h"

#include "internal/utils.h"

using namespace rx::abstract;

namespace rx::scheduler {

class GodotSignalScheduler : public GodotSignalSchedulerBase, public Scheduler {

protected:
    GodotSignalScheduler() {}
public:
    ~GodotSignalScheduler(){}
    inline std::shared_ptr<GodotSignalScheduler> getptr() { return std::static_pointer_cast<GodotSignalScheduler>(Scheduler::getptr()); }
    inline static std::shared_ptr<GodotSignalScheduler> get() 
    { 
        return std::shared_ptr<GodotSignalScheduler>(new GodotSignalScheduler()); 
    }

    static std::shared_ptr<GodotSignalScheduler> singleton();

    disposable_t schedule(const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_absolute(const time_point_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_relative(const time_delta_t& duetime, const scheduled_action_t& action, const Variant& state = Variant()) override;
    disposable_t schedule_signal(const Object* owner, const StringName& signal_name, const periodic_action_t& action, const Variant& state = Variant()) override;
};

class RxSignalCallback : public RefCounted {
    GDCLASS(RxSignalCallback, RefCounted)

protected:
    inline static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("_callback0"), &RxSignalCallback::_callback0);
        ClassDB::bind_method(D_METHOD("_callback1"), &RxSignalCallback::_callback1);
        ClassDB::bind_method(D_METHOD("_callback2"), &RxSignalCallback::_callback2);
        ClassDB::bind_method(D_METHOD("_callback3"), &RxSignalCallback::_callback3);
        ClassDB::bind_method(D_METHOD("_callback4"), &RxSignalCallback::_callback4);
        ClassDB::bind_method(D_METHOD("_callback5"), &RxSignalCallback::_callback5);
        ClassDB::bind_method(D_METHOD("_callback6"), &RxSignalCallback::_callback6);
        ClassDB::bind_method(D_METHOD("_callback7"), &RxSignalCallback::_callback7);
        ClassDB::bind_method(D_METHOD("_callback8"), &RxSignalCallback::_callback8);
        ClassDB::bind_method(D_METHOD("_callback9"), &RxSignalCallback::_callback9);
        ClassDB::bind_method(D_METHOD("_callback10"), &RxSignalCallback::_callback10);
        ClassDB::bind_method(D_METHOD("_callback11"), &RxSignalCallback::_callback11);
        ClassDB::bind_method(D_METHOD("_callback12"), &RxSignalCallback::_callback12);
        ClassDB::bind_method(D_METHOD("_callback13"), &RxSignalCallback::_callback13);
        ClassDB::bind_method(D_METHOD("_callback14"), &RxSignalCallback::_callback14);
        ClassDB::bind_method(D_METHOD("_callback15"), &RxSignalCallback::_callback15);
        ClassDB::bind_method(D_METHOD("_callback16"), &RxSignalCallback::_callback16);
    }

private:
    periodic_action_t _action;

public:
    RxSignalCallback() { throw NotImplementedException(); }
    RxSignalCallback(const periodic_action_t& action) 
        : _action(action) {}
    ~RxSignalCallback(){}

    /* Callback functions. Support up to 16 arguments */
    /* Why not use default arguments and NotSet? Good question... */
    /* Dunno, but this will do for now. */

    inline Variant _callback0() {
        return _action(VNULL);
    }
    inline Variant _callback1(const Variant& a1) {
        return _action(a1);
    }
    inline Variant _callback2(const Variant& a1, const Variant& a2) {
        return _action(Array::make(a1, a2));
    }
    inline Variant _callback3(const Variant& a1, const Variant& a2, const Variant& a3) {
        return _action(Array::make(a1, a2, a3));
    }
    inline Variant _callback4(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4
    ) {
        return _action(Array::make(a1, a2, a3, a4));
    }
    inline Variant _callback5(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5));
    }
    inline Variant _callback6(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6));
    }
    inline Variant _callback7(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7));
    }
    inline Variant _callback8(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8));
    }
    inline Variant _callback9(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }
    inline Variant _callback10(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }
    inline Variant _callback11(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));
    }
    inline Variant _callback12(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11,
        const Variant& a12
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));
    }
    inline Variant _callback13(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11,
        const Variant& a12,
        const Variant& a13
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13));
    }
    inline Variant _callback14(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11,
        const Variant& a12,
        const Variant& a13,
        const Variant& a14
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14));
    }
    inline Variant _callback15(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11,
        const Variant& a12,
        const Variant& a13,
        const Variant& a14,
        const Variant& a15
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15));
    }
    inline Variant _callback16(
        const Variant& a1, 
        const Variant& a2, 
        const Variant& a3,
        const Variant& a4,
        const Variant& a5,
        const Variant& a6,
        const Variant& a7,
        const Variant& a8,
        const Variant& a9,
        const Variant& a10,
        const Variant& a11,
        const Variant& a12,
        const Variant& a13,
        const Variant& a14,
        const Variant& a15,
        const Variant& a16
    ) {
        return _action(Array::make(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16));
    }

};

} // END namespace rx::scheduler

#endif // RX_SCHEDULER_GODOTSIGNALSCHEDULER_H
