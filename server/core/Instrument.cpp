#include <algorithm>
#include "Instrument.h"

Instrument::~Instrument(){}
void Instrument::add_note(Note& note)
{
    notes.push_back(note);
    std::sort(notes.begin(),notes.end());
}

double Instrument::omega(const Note& n) const
{
    return 2 * M_PI * frequency(n) / Player::sample_rate;
}

double Instrument::frequency(const Note& n) const
{
    return Player::freq_reference * pow(2,n.octave()+(static_cast<int>(n.pitch_class())/12.0));
}

Player::spPacket Instrument::get_samples(int begin, int end)
{
    //find the first note that ends after sample count "begin"
    auto begin_note_it = std::lower_bound(notes.begin(), notes.end(), begin,
            [](const Note& n,const int& i){
                return n.off() > i;
            });

    //find the first note that begins after sample count "end"
    auto end_note_it = std::upper_bound(notes.begin(), notes.end(), end,
            [](const int& i, const Note& n){
                return n.on() > i;
            });    

    Player::spPacket ret(new Player::Packet(end-begin,0));
    std::for_each(begin_note_it,end_note_it,[this,ret,begin](Note& note)
            {
                this->generate(note,*ret,begin);
            });
    return ret;
}
void Instrument::generate(Note& note, Player::Packet& p, int start_sample)
{
    const int end_sample=start_sample+p.size();
    int on = (note.on() > start_sample) ? note.on() : start_sample; 
    int off = (note.off() < end_sample) ? note.off() : end_sample;
    this->gen(note,p,on,off);
}
