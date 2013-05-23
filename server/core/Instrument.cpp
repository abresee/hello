#include <cmath>
#include <algorithm>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Instrument.h"

using namespace Config;

Instrument::~Instrument(){}
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

PacketHandle Instrument::get_samples(int begin, int end)
{
    //find the first note that ends after sample count "begin"
    auto begin_note_iter = std::lower_bound(notes.begin(), notes.end(), begin,
            [](const Note& n,const int& i){
                return n.off() > i;
            });

    //find the first note that begins after sample count "end"
    auto end_note_iter = std::upper_bound(notes.begin(), notes.end(), end,
            [](const int& i, const Note& n){
                return n.on() > i;
            });    

    PacketHandle ret(new Packet(end-begin,0));
    std::for_each(begin_note_iter,end_note_iter,[this,ret,begin](Note& note)
            {
                this->generate(note,*ret,begin);
            });
    return ret;
}

void Instrument::generate(Note& note, Packet& p, int start_sample)
{
    const int end_sample=start_sample+p.size();
    int on = (note.on() > start_sample) ? note.on() : start_sample; 
    int off = (note.off() < end_sample) ? note.off() : end_sample;
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
