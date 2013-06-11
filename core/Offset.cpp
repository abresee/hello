#include "Offset.h"
#include "Beat.h"
#include "Time.h"
#include "global.h"

Offset::Offset():
    offset_(){
}

Offset::Offset(underlying offset_initializer):
    offset_(offset_initializer) {
}
//offset(samples) * 1/sample_rate (secs/samples) * 1e9 ns/second = time
//offset(samples)   |             (secs)    | 1e9 (ns)
//                  | sample_rate (samples) | 1   (secs)
Time Offset::to_time(const Offset& sample_rate) const {
    return Time(offset_*Config::count_ns_in_sec/sample_rate.as_double());}

//offset(samples) * 1/sample_rate (secs/samples) * tempo (beats/secs) = Beat
//offset(samples)   |             (secs)    | tempo (beats)
//                  | sample_rate (samples) |       (secs)

Beat Offset::to_beat(const Offset& sample_rate, const Beat& tempo) const {
    return Beat(offset_*tempo.as_double()/sample_rate.as_double());
}

bool Offset::operator<(const Offset& other) const {
    return offset_ < other.offset_;
}

bool Offset::operator<=(const Offset& other) const {
    return offset_ <= other.offset_;
}

bool Offset::operator>(const Offset& other) const {
    return other <= *this;
}

bool Offset::operator>=(const Offset& other) const {
    return other < *this;
}

bool Offset::operator==(const Offset& other) const {
    return offset_ == other.offset_;
}

Offset Offset::operator-(const Offset& other) const {
    return offset_-other.offset_;
}

Offset Offset::operator+(const Offset& other) const {
    return offset_+other.offset_;
}

Offset Offset::operator+(const Offset::underlying& other) const {
    return Offset(offset_+other);
}

Offset Offset::operator+=(const Offset::underlying& other) {
    offset_+=other;
    return *this;
}

Offset Offset::operator*(const Offset::underlying& other) const {
    return Offset(offset_*other);
}

Offset::underlying Offset::value() const {
    return offset_;
}

double Offset::as_double() const {
    return static_cast<double>(offset_);
}

Offset operator*(const Offset::underlying& lhs, const Offset& rhs) {
    return rhs*lhs;
}
