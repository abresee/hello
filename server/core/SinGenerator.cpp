#include "SinGenerator.h"
Player::spPacket SinGenerator::get_samples(int sample_count)
{
    Player::spPacket ret(new Player::packet_t(sample_count));    
    for(int i=0; i < sample_count; ++i)
    {
        Player::sample_t sample=amplitude*sin(omega*(i+total_samples));
       (*ret)[i]=sample;
    }
    total_samples+=sample_count;
    return ret;
}

void SinGenerator::add_note(spNote& note)
{
    Instrument::add_note(note);    
}
