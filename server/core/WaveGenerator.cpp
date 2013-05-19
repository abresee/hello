#include "WaveGenerator.h"

WaveGenerator::WaveGenerator(std::function<Player::Sample(double)> wave__) : wave_(wave__){}

void WaveGenerator::gen(Note& note, Player::Packet& p,int on, int off)
{
    for(int i=on;i<off;++i)
    {
        p[i]+=note.volume()*wave_(omega(note)*i); 
    }
}

std::function<Player::Sample(double)>& WaveGenerator::wave()
{
    return wave_;
}

void WaveGenerator::add_note(Note& note)
{
    Instrument::add_note(note);    
}
