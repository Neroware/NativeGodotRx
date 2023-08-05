#include "internal/time.h"

using namespace rx;

Ref<AbsoluteTime> AbsoluteTime::from_seconds(double t) {
    return memnew(AbsoluteTime(t));
}

Ref<AbsoluteTime> AbsoluteTime::utc_zero() {
    return memnew(AbsoluteTime(UTC_ZERO));
}

void AbsoluteTime::_bind_methods() {
    ClassDB::bind_static_method("AbsoluteTime", D_METHOD("from_seconds", "t_sec"), &AbsoluteTime::from_seconds);
    ClassDB::bind_static_method("AbsoluteTime", D_METHOD("UTC_ZERO"), &AbsoluteTime::utc_zero);
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

bool AbsoluteTime::operator==(const AbsoluteTime& other) const {
    return this->t == other.t;
}
bool AbsoluteTime::operator<(const AbsoluteTime& other) const {
    return this->t < other.t;
}
bool AbsoluteTime::operator>(const AbsoluteTime& other) const {
    return this->t > other.t;
}
bool AbsoluteTime::operator<=(const AbsoluteTime& other) const {
    return this->t < other.t;
}
bool AbsoluteTime::operator>=(const AbsoluteTime& other) const {
    return this->t > other.t;
}
Ref<RelativeTime> AbsoluteTime::operator-(const AbsoluteTime& other) const {
    return memnew(RelativeTime(this->t - other.t));
}
Ref<AbsoluteTime> AbsoluteTime::operator+(const RelativeTime& other) const {
    return memnew(AbsoluteTime(this->t + other.dt));
}

Ref<RelativeTime> RelativeTime::from_seconds(double t) {
    return memnew(RelativeTime(t));
}

Ref<RelativeTime> RelativeTime::delta_zero() {
    return memnew(RelativeTime(DELTA_ZERO));
}

void RelativeTime::_bind_methods() {
    ClassDB::bind_static_method("RelativeTime", D_METHOD("from_seconds", "dt_sec"), &RelativeTime::from_seconds);
    ClassDB::bind_static_method("RelativeTime", D_METHOD("DELTA_ZERO"), &RelativeTime::delta_zero);
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

bool RelativeTime::operator==(const RelativeTime& other) const {
    return this->dt == other.dt;
}
bool RelativeTime::operator<(const RelativeTime& other) const {
    return this->dt < other.dt;
}
bool RelativeTime::operator>(const RelativeTime& other) const {
    return this->dt > other.dt;
}
bool RelativeTime::operator<=(const RelativeTime& other) const {
    return this->dt <= other.dt;
}
bool RelativeTime::operator>=(const RelativeTime& other) const {
    return this->dt >= other.dt;
}
Ref<RelativeTime> RelativeTime::operator-(const RelativeTime& other) const {
    return memnew(RelativeTime(this->dt - other.dt));
}
Ref<RelativeTime> RelativeTime::operator+(const RelativeTime& other) const {
    return memnew(RelativeTime(this->dt + other.dt));
}