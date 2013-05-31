#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Instrument.h"

Instrument::~Instrument(){}

void Instrument::get_samples(Packet& p, const guint64 start_offset)
{
    const guint64 end_offset = start_offset+ p.size();
    std::vector<Note> notes_to_gen;
    for(Note note : notes)
    {
        if(note.off() > start_offset && note.on() < end_offset)
        { 
            notes_to_gen.push_back(note);
        }
    }
    
    for(Note note : notes_to_gen)
    {
        this->gen(note,p,start_offset);
    }
}

void Instrument::add_note(const Note note)
{
    notes.push_back(note);
    std::sort(notes.begin(),notes.end());
}

void Instrument::add_notes(const std::vector<Note> new_notes)
{
    notes.insert(notes.end(),new_notes.begin(),new_notes.end());
    std::sort(notes.begin(),notes.end());
}

double Instrument::omega(const Note n) const
{
    return  2 * M_PI * frequency(n) / Config::sample_rate;
}

double Instrument::frequency(const Note n) const
{
    auto exponent = (Config::pc_count*n.octave()+n.pitch_class())/static_cast<double>(Config::pc_count);
    return Config::freq_reference * pow(2,exponent);
}

guint64 Instrument::stream_end() const
{
    return notes.back().off(); 
}

Sample Instrument::round(double t)
{
    using namespace boost::numeric;
    converter<Sample
        , double
        , conversion_traits<Sample, double>
        , def_overflow_handler
        , Trunc<typename conversion_traits<Sample, double>::source_type>
        > typeconverter;    
    return typeconverter(t); 
}
