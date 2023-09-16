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

class RxTimeStamp : public RefCounted {
    GDCLASS(RxTimeStamp, RefCounted)

private:
    Ref<AbsoluteTime> _timestamp;
    Variant _value;

protected:
    inline static void _bind_methods() {
        ClassDB::bind_static_method("RxTimeStamp", D_METHOD("from_abs", "timestamp", "value"), &RxTimeStamp::from_abs, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("_get_value"), &RxTimeStamp::get_value);
        ClassDB::bind_method(D_METHOD("_get_timestamp"), &RxTimeStamp::get_timestamp);
        ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "value"), "", "_get_value");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "timestamp"), "", "_get_timestamp");
    }

public:
    RxTimeStamp() {}
    RxTimeStamp(Ref<AbsoluteTime> timestamp, const Variant& value)
        : _timestamp(timestamp), _value(value) {}
    inline static Ref<RxTimeStamp> from_abs(Ref<AbsoluteTime> timestamp, const Variant& value = Variant()) {
        return memnew(RxTimeStamp(timestamp, value));
    }
    inline Variant get_value() { return this->_value; }
    inline Ref<AbsoluteTime> get_timestamp() { return this->_timestamp; }
    
}; // END class RxTimeStamp

template<typename T>
struct timestamp_t {
    time_point_t timestamp;
    T value;
    timestamp_t(const time_point_t& timestamp_, const Variant& value_)
        : timestamp(timestamp_), value(value_) {}
    timestamp_t(const Variant& timestamp_)
        : timestamp_t(Ref<RxTimeStamp>(timestamp_)) {}
    timestamp_t(Ref<RxTimeStamp> timestamp_)
        : timestamp(timestamp_->get_timestamp()->t), value(timestamp_->get_value()) {}
    operator Ref<RxTimeStamp>() const {
        return memnew(RxTimeStamp(
            memnew(AbsoluteTime(timestamp)),
            value
        ));
    }
    operator Variant() const {
        return operator godot::Ref<rx::RxTimeStamp>();
    }
    
}; // END struct timestamp_t

class RxTimeInterval : public RefCounted {
    GDCLASS(RxTimeInterval, RefCounted)

private:
    Ref<RelativeTime> _interval;
    Variant _value;

protected:
    inline static void _bind_methods() {
        ClassDB::bind_static_method("RxTimeInterval", D_METHOD("from_rel", "interval", "value"), &RxTimeInterval::from_rel, DEFVAL(Variant()));
        ClassDB::bind_method(D_METHOD("_get_value"), &RxTimeInterval::get_value);
        ClassDB::bind_method(D_METHOD("_get_interval"), &RxTimeInterval::get_interval);
        ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "value"), "", "_get_value");
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "interval"), "", "_get_interval");
    }

public:
    RxTimeInterval() {}
    RxTimeInterval(Ref<RelativeTime> interval, const Variant& value)
        : _interval(interval), _value(value) {}
    inline static Ref<RxTimeInterval> from_rel(Ref<RelativeTime> interval, const Variant& value = Variant()) {
        return memnew(RxTimeInterval(interval, value));
    }
    inline Variant get_value() { return this->_value; }
    inline Ref<RelativeTime> get_interval() { return this->_interval; }
    
}; // END class RxTimeInterval

template<typename T>
struct interval_t {
    time_delta_t interval;
    T value;
    interval_t(const time_delta_t& interval_, const Variant& value_)
        : interval(interval_), value(value_) {}
    interval_t(const Variant& interval_)
        : interval_t(Ref<RxTimeStamp>(interval_)) {}
    interval_t(Ref<RxTimeInterval> interval_)
        : interval(interval_->get_interval()->dt), value(interval_->get_value()) {}
    operator Ref<RxTimeInterval>() const {
        return memnew(RxTimeInterval(
            memnew(RelativeTime(interval)),
            value
        ));
    }
    operator Variant() const {
        return operator godot::Ref<rx::RxTimeInterval>();
    }
    
}; // END struct interval_t

}; // END namespace rx

#endif // RX_TIME_H