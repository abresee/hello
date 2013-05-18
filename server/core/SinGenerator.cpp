#include "SinGenerator.h"
Player::spPacket SinGenerator::generate(Note& note)
{
    Player::spPacket ret(new Player::Packet(note.length()));    
    for(int i=0; i < note.length(); ++i)
    {
        Player::Sample sample=amplitude*sin(omega*(i+note.on()));
       (*ret)[i]=sample;
    }
    return ret;
}

void SinGenerator::add_note(Note& note)
{
    Instrument::add_note(note);    
}
