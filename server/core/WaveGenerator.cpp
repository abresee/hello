#include <cmath>
#include <iostream>
#include "WaveGenerator.h"

WaveGenerator::WaveGenerator(std::function<double(double)> wave_I) : wave_(wave_I){}

void WaveGenerator::gen(Note& note, Packet& p, int on, int off)
{
    std::cout<<"WaveGenerator::gen "<<p.size()<<" "<<off-on<<std::endl;
    for(int i=on;i<off;++i)
    {
        p.at(i)+=round(note.intensity()*wave_(omega(note)*i)); 
    }
}

std::function<double(double)>& WaveGenerator::wave()
{
    return wave_;
}

void WaveGenerator::add_note(Note& note)
{
    Instrument::add_note(note);    
}
