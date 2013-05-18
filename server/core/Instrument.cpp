#include <algorithm>
#include "Instrument.h"

Instrument::~Instrument(){}
void Instrument::add_note(Note& note)
{
    notes.push_back(note);
    std::sort(notes.begin(),notes.end());
}

Player::spPacket Instrument::get_samples(int begin, int end)
{
    //find the first note that starts after sample count "begin"
    auto begin_note_it = std::lower_bound(notes.begin(), notes.end(), begin,
            [](const Note& n,const int& i){
                return n.on() < i;
            });

    auto end_note_it = std::upper_bound(notes.begin(), notes.end(), end,
            [](const int& i, const Note& n){
                return i < n.on();
            });    

    Player::spPacket ret(new Player::Packet(end-begin,0));
    std::for_each(begin_note_it,end_note_it,[this,ret](Note& note)
            {
                Player::spPacket note_samples = this->generate(note);
                std::copy(
                    note_samples->begin(),
                    note_samples->end(),
                    ret->begin()+note.on()); 
            });
    return ret;
}
