#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H 
#include <functional>
#include "Config.h"
#include "Instrument.h"
/// @brief instrument that generates a sin wave
class WaveGenerator : public Instrument
{
    std::function<double(double)> wave_;
    virtual void gen(Note& note, Packet& p, int on, int off);
public:
    WaveGenerator(std::function<double(double)>);
    std::function<double(double)>& wave();
    virtual void add_note(Note&);
};
#endif /* WAVEGENERATOR_H */
