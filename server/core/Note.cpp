#include "Note.h"

Note::Note() : Note(PitchClass::Do,0,0,0) {}
Note::Note(PitchClass p_, int octave_, int start_, int end_) :
    p(p_), octave(octave_), start(start_), end(end_) {}
    
Note::PitchClass Note::get_pitch_class() const
{
    return p;
}

int Note::get_octave() const
{
    return octave;
}

int Note::get_start() const
{
    return start;
}

int Note::get_end() const 
{
    return end;
}
