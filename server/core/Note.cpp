#include <iostream>
#include "Note.h"

Note::Note(int pc, int o, Sample i, guint64 note_on, guint64 note_off) :
    pitch_class_(pc), octave_(o), intensity_(i), on_(note_on), off_(note_off)
{
    using Config::pc_count;
    if (pitch_class_ < 0)
    {
        octave_ += pitch_class_ / pc_count - 1; 
        pitch_class_=(pitch_class_ % pc_count)+pc_count;
    }
    else if (pitch_class_ > 11)
    {
        octave_ += pitch_class_ / pc_count;
        pitch_class_%=pc_count;
    }
}
    
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

bool Note::operator==(const Note& other) const
{
    return (pitch_class_ < other.pitch_class_) &&
        (octave_ < other.octave_) &&
        (intensity_ < other.intensity_) &&
        (on_ < other.on_) &&
        (off_ < other.off_ ) ;
}
