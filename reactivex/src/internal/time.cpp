#include "time.h"

AbsoluteTime::AbsoluteTime() : t(basic::now<time_point_t>()) {}
AbsoluteTime::AbsoluteTime(time_point_t t) : t(t) {}
AbsoluteTime::~AbsoluteTime() {}

Ref<AbsoluteTime> AbsoluteTime::Get(double t) {
    std::chrono::nanoseconds shift_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(t));
    return memnew(AbsoluteTime(UTC_ZERO + shift_ns));
}

void AbsoluteTime::_bind_methods() {
    ClassDB::bind_static_method("AbsoluteTime", D_METHOD("Get", "t_sec"), &AbsoluteTime::Get);
    ClassDB::bind_method(D_METHOD("to_sec"), &AbsoluteTime::to_sec);
    ClassDB::bind_method(D_METHOD("time_since_epoch"), &AbsoluteTime::time_since_epoch);
    ClassDB::bind_method(D_METHOD("timeshift", "dt"), &AbsoluteTime::timeshift);
    ClassDB::bind_method(D_METHOD("timedelta", "t"), &AbsoluteTime::timedelta);
}

double AbsoluteTime::to_sec() {
    std::chrono::duration<double> duration_since_epoch = this->t.time_since_epoch();
    return duration_since_epoch.count();
}

Ref<RelativeTime> AbsoluteTime::time_since_epoch() {
    return memnew(RelativeTime(t.time_since_epoch()));
}

Ref<AbsoluteTime> AbsoluteTime::timeshift(Ref<RelativeTime> dt) {
    return memnew(AbsoluteTime(this->t + dt->dt));
}

Ref<RelativeTime> AbsoluteTime::timedelta(Ref<AbsoluteTime> t) {
    return memnew(RelativeTime(t->t - this->t));
}


RelativeTime::RelativeTime() : dt(basic::now<time_point_t>().time_since_epoch()) {}
RelativeTime::RelativeTime(time_delta_t dt) : dt(dt) {}
RelativeTime::~RelativeTime() {}

Ref<RelativeTime> RelativeTime::Get(double t) {
    std::chrono::nanoseconds shift_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(t));
    return memnew(RelativeTime(DELTA_ZERO + shift_ns));
}

void RelativeTime::_bind_methods() {
    ClassDB::bind_static_method("RelativeTime", D_METHOD("Get", "dt_sec"), &RelativeTime::Get);
    ClassDB::bind_method(D_METHOD("to_sec"), &RelativeTime::to_sec);
    ClassDB::bind_method(D_METHOD("timeshift", "dt"), &RelativeTime::timeshift);
    ClassDB::bind_method(D_METHOD("timedelta", "t"), &RelativeTime::timedelta);
}

double RelativeTime::to_sec() {
    std::chrono::duration<double> duration = this->dt;
    return duration.count();
}

Ref<RelativeTime> RelativeTime::timedelta(Ref<RelativeTime> dt) {
    return memnew(RelativeTime(dt->dt - this->dt));
}

Ref<AbsoluteTime> RelativeTime::timeshift(Ref<AbsoluteTime> t) {
    return memnew(AbsoluteTime(t->t + this->dt));
}