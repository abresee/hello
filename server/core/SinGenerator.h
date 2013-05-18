#ifndef SINGENERATOR_H
#define SINGENERATOR_H 
#include "Instrument.h"
#include "Player.h"
/// @brief instrument that generates a sin wave
class SinGenerator : public Instrument
{
public:
    /// @brief ctor. default ctor chains to this one.
    SinGenerator(int amplitude_, double omega_) : amplitude(amplitude_), omega(omega_) {}
    /// @brief ctor. chains.
    SinGenerator() : amplitude(0), omega(0.0) {}

    void set_amplitude(int amplitude_){amplitude=amplitude_;}
    void set_omega(double omega_){omega=omega_;}
    void set_frequency(double frequency){omega=2*M_PI*frequency;}
    virtual void add_note(Note&);
    virtual Player::spPacket generate(Note&);

private:
    int amplitude;
    double omega;
};
#endif /* SINGENERATOR_H */
