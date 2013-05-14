#ifndef SINGENERATOR_H
#define SINGENERATOR_H 
#include "Instrument.h"
/// @brief instrument that generates a sin wave
class SinGenerator : public Instrument
{
public:
    /// @brief ctor. default ctor chains to this one.
    SinGenerator(int amplitude_, double omega_) : amplitude(amplitude_), omega(omega_) {}
    /// @brief ctor. chains.
    SinGenerator() : amplitude(0), omega(0.0) {}
    /// @brief returns a packet of size sample_count by shared_ptr 
    virtual Player::spPacket get_samples(int sample_count); 

    void set_amplitude(int amplitude_){amplitude=amplitude_;}
    void set_omega(double omega_){omega=omega_;}
    void set_frequency(double frequency){omega=2*M_PI*frequency;}

private:
    int amplitude;
    double omega;
    int total_samples;
};
#endif /* SINGENERATOR_H */
