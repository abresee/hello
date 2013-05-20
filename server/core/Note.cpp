#include "Note.h"

Note::Note(int pitch_class_I, Sample intensity_I, int octave_I, int on_I, int off_I) :
    pitch_class_(pitch_class_I),intensity_(intensity_I), octave_(octave_I), on_(on_I), off_(off_I) {}
    
int Note::pitch_class() const
{
    return pitch_class_;
}

Sample Note::intensity() const
{
    return intensity_;
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
