#include <iostream>
#include "Note.h"

Note::Note(int pc, Sample i, int o, guint64 b, guint64 e) :
    pitch_class_(pc),intensity_(i), octave_(o), on_(b), off_(e) {}
    
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

guint64 Note::on() const
{
    return on_;
}

guint64 Note::off() const 
{
    return off_;
}

guint64 Note::length() const
{
    return off_-on_;
}

bool Note::operator<(const Note& other) const
{
    return on_<other.on_;
}
