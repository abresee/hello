#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H 
#include <functional>
#include "Config.h"
#include "Instrument.h"
/// @brief instrument that generates a sin wave
class WaveGenerator : public Instrument {
    std::function<double(double)> wave_;
    virtual void gen(const Note note, Packet& p, const guint64 start_offset);
public:
    WaveGenerator(std::function<double(double)> wave_I);
    WaveGenerator();
    std::function<double(double)>& wave();
};
#endif /* WAVEGENERATOR_H */
