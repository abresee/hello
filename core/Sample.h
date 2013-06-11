#ifndef SAMPLE_H
#define SAMPLE_H 
#include <cstdint>
#include <ostream>
class Sample {
    int16_t sample_;
public:
    Sample();
    Sample(int16_t sample_initializer);
    typedef int16_t underlying;
    underlying value() const;
    const static Sample max_intensity;
    bool operator==(const Sample& other) const;
    bool operator<(const Sample& other) const;
    bool operator>(const Sample& other) const;
    Sample operator+(const Sample& other) const;
    Sample& operator+=(const Sample& other);
    Sample operator/(const underlying& other) const;
    friend std::ostream& operator<<(std::ostream& stream, Sample sample);
};
#endif /* SAMPLE_H */
