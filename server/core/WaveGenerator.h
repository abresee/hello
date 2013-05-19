#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H 
#include <functional>
#include "Instrument.h"
#include "Player.h"
/// @brief instrument that generates a sin wave
class WaveGenerator : public Instrument
{
    std::function<Player::Sample(double)> wave_;
    virtual void gen(Note&, Player::Packet&, int, int);
public:
    WaveGenerator(std::function<Player::Sample(double)>);
    std::function<Player::Sample(double)>& wave();
    virtual void add_note(Note&);
};
#endif /* WAVEGENERATOR_H */
