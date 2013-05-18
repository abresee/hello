#include "Note.h"

Note::Note() : Note(PitchClass::Do,0,0,0) {}
Note::Note(PitchClass p__, int octave__, int on__, int off__) :
    p_(p__), octave_(octave__), on_(on__), off_(off__) {}
    
Note::PitchClass Note::pitch_class() const
{
    return p_;
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
