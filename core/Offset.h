#ifndef OFFSET_H
#define OFFSET_H 
#include <cstdint>
class Time;
class Beat;
class Offset {
    int64_t offset_;
public:
    typedef int64_t underlying;
    Offset();
    Offset(underlying offset_initializer);
    Time to_time(const Offset& sample_rate) const;
    Beat to_beat(const Offset& sample_rate, const Beat& tempo) const;
    bool operator<(const Offset& other) const;
    bool operator<=(const Offset& other) const;
    bool operator>(const Offset& other) const;
    bool operator>=(const Offset& other) const;
    bool operator==(const Offset& other) const;
    Offset operator-(const Offset& other) const;
    Offset operator+(const Offset& other) const;
    Offset operator+(const underlying& other) const;
    Offset operator+=(const underlying& other);
    Offset operator*(const underlying& other) const;
    underlying value() const;
    double as_double() const;
};

Offset operator*(const Offset::underlying& lhs, const Offset& rhs);
#endif /* OFFSET_H */
