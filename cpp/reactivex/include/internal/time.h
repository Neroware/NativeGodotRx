#ifndef RX_TIME_H
#define RX_TIME_H

#include <chrono>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace rx {

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
    AbsoluteTime() : AbsoluteTime(0.0) {}
    AbsoluteTime(time_point_t t) : t(t){}
    AbsoluteTime(double t) : t(UTC_ZERO + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(t))) {
    }
    ~AbsoluteTime(){}

    static Ref<AbsoluteTime> from_seconds(double t);
    
    static Ref<AbsoluteTime> utc_zero();

    double to_sec();
    Ref<RelativeTime> time_since_epoch();

    Ref<AbsoluteTime> timeshift(Ref<RelativeTime> dt);
    Ref<RelativeTime> timedelta(Ref<AbsoluteTime> t);

    bool operator==(const AbsoluteTime& other) const;
    bool operator<(const AbsoluteTime& other) const;
    bool operator>(const AbsoluteTime& other) const;
    bool operator<=(const AbsoluteTime& other) const;
    bool operator>=(const AbsoluteTime& other) const;
    Ref<RelativeTime> operator-(const AbsoluteTime& other) const;
    Ref<AbsoluteTime> operator+(const RelativeTime& other) const;

}; // END AbsoluteTime


class RelativeTime : public RefCounted {
    GDCLASS(RelativeTime, RefCounted);

public:
    time_delta_t dt;

protected:
	static void _bind_methods();

public:
    RelativeTime() : RelativeTime(0.0) {}
    RelativeTime(time_delta_t dt) : dt(dt) {}
    RelativeTime(double t) : dt(DELTA_ZERO + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(t))) {}
    ~RelativeTime() {}

    static Ref<RelativeTime> from_seconds(double dt);

    static Ref<RelativeTime> delta_zero();

    double to_sec();
    
    Ref<RelativeTime> timedelta(Ref<RelativeTime> dt);
    Ref<AbsoluteTime> timeshift(Ref<AbsoluteTime> t);

    bool operator==(const RelativeTime& other) const;
    bool operator<(const RelativeTime& other) const;
    bool operator>(const RelativeTime& other) const;
    bool operator<=(const RelativeTime& other) const;
    bool operator>=(const RelativeTime& other) const;
    Ref<RelativeTime> operator-(const RelativeTime& other) const;
    Ref<RelativeTime> operator+(const RelativeTime& other) const;

}; // END RelativeTime

}; // END namespace rx

#endif // RX_TIME_H