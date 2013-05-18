#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <vector>
#include "Player.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    Player::spPacket get_samples(int begin,int end); 
    virtual ~Instrument();
    virtual void add_note(Note&);

private:
    virtual Player::spPacket generate(Note&)=0;
    std::vector<Note> notes;
};
#endif /* INSTRUMENT_H */
