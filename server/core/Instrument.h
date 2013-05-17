#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <vector>
#include "Player.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    typedef boost::shared_ptr<Note> spNote;
    virtual Player::spPacket get_samples(int sample_count)=0; 
    virtual ~Instrument();
    virtual void add_note(spNote&);

private:
    std::vector<spNote> notes;
};
#endif /* INSTRUMENT_H */
