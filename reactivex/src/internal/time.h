#ifndef RX_TIME_H
#define RX_TIME_H

#include <chrono>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "internal/basic.h"

class AbsoluteTime;
class RelativeTime;

typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
typedef std::chrono::time_point<std::chrono::system_clock>::duration time_delta_t;

const time_point_t UTC_ZERO = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>(std::chrono::nanoseconds(0));
const time_delta_t DELTA_ZERO = UTC_ZERO - UTC_ZERO;

using namespace godot;


class AbsoluteTime : public RefCounted {
    GDCLASS(AbsoluteTime, RefCounted);

public:
    time_point_t t;

protected:
	static void _bind_methods();

public:
    AbsoluteTime();
    AbsoluteTime(time_point_t t);
    ~AbsoluteTime();

    static Ref<AbsoluteTime> Get(double t);

    double to_sec();
    Ref<RelativeTime> time_since_epoch();

    Ref<AbsoluteTime> timeshift(Ref<RelativeTime> dt);
    Ref<RelativeTime> timedelta(Ref<AbsoluteTime> t);

}; // END AbsoluteTime


class RelativeTime : public RefCounted {
    GDCLASS(RelativeTime, RefCounted);

public:
    time_delta_t dt;

protected:
	static void _bind_methods();

public:
    RelativeTime();
    RelativeTime(time_delta_t dt);
    ~RelativeTime();

    static Ref<RelativeTime> Get(double dt);

    double to_sec();
    
    Ref<RelativeTime> timedelta(Ref<RelativeTime> dt);
    Ref<AbsoluteTime> timeshift(Ref<AbsoluteTime> t);

}; // END RelativeTime

#endif // RX_TIME_H