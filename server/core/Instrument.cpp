#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Instrument.h"

using namespace Config;
using std::cout;
using std::endl;

Instrument::~Instrument(){}

PacketHandle Instrument::get_samples(int begin, int end)
{
    cout<<"Instrument::get_samples"<<endl;
    PacketHandle ret(new Packet(end-begin,0));
    std::vector<Note> notes_to_gen;
    for(Note note : notes)
    {
        if(note.off() > begin && note.on() < end)
        {
            notes_to_gen.push_back(note);
        }
    }
    
    cout<<"found "<<notes_to_gen.size()<<endl;

    for(Note note : notes_to_gen)
    {
        cout<<"calling to generate"<<endl;
        generate(note,*ret,begin);
    }
    return ret;
}

void Instrument::add_note(Note& note)
{
    notes.push_back(note);
    std::sort(notes.begin(),notes.end());
}

double Instrument::omega(const Note& n) const
{
    return 2 * M_PI * frequency(n) / sample_rate;
}

double Instrument::frequency(const Note& n) const
{
    return Config::freq_reference * pow(2,n.octave()+(static_cast<int>(n.pitch_class())/12.0));
}

guint64 Instrument::stream_end() const
{
    return notes.back().off(); 
}

void Instrument::generate(Note& note, Packet& p,const int start_offset)
{
    cout<<"Instrument::generate"<<endl;
    const int end_offset=start_offset+p.size();
    const int on = (note.on() > start_offset) ? note.on() : start_offset;
    const int off = (note.off() < end_offset) ? note.off() : end_offset;
    this->gen(note,p,on,off);
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
