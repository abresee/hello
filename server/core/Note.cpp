#include "Note.h"

Note::Note(int pitch_classI, Player::Sample intensityI, int octaveI, int onI, int offI) :
    pitch_class_(pitch_classI),volume_(volumeI), octave_(octaveI), on_(onI), off_(offI) {}
    
int Note::pitch_class() const
{
    return pitch_class_;
}

Player::Sample Note::volume() const
{
    return volume_;
}

int Note::octave() const
{
    return octave_;
}

int Note::on() const
{
    return on_;
}

int Note::off() const 
{
    return off_;
}

int Note::length() const
{
    return off_-on_;
}

bool Note::operator<(const Note& other) const
{
    return on_<other.on_;
}
