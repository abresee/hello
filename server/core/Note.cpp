#include "Note.h"

Note::Note(int pitch_class__, Player::Sample volume__, int octave__, int on__, int off__) :
    pitch_class_(pitch_class__),volume_(volume__), octave_(octave__), on_(on__), off_(off__) {}
    
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
