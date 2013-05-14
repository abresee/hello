#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include "Player.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    virtual Player::spPacket get_samples(int sample_count)=0; 
    virtual ~Instrument(){}
};
#endif /* INSTRUMENT_H */
