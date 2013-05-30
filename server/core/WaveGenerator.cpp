#include <cmath>
#include <iostream>
#include <stdexcept>
#include "WaveGenerator.h"

WaveGenerator::WaveGenerator(std::function<double(double)> wave_I) : wave_(wave_I){}

void WaveGenerator::gen(const Note note, Packet& p, const guint64 start_offset)
{
    const guint64 end_offset = start_offset;
    const guint64 on = (note.on() > start_offset) ? (note.on() - start_offset) : (0);
    const guint64 off = (note.off() < end_offset) ? (note.off() - start_offset) : (p.size()); 
    for(auto i=on;i<off;++i)
    {
        p[i]+=round(note.intensity()*wave_(omega(note) * (i+start_offset))); 
    }
}

std::function<double(double)>& WaveGenerator::wave()
{
    return wave_;
}
