#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
class Note 
{
public:
    enum PitchClass 
    {
        Do=0,
        Rah,
        Re,
        Me,
        Mi,
        Fa,
        Fi,
        Sol,
        Le,
        La,
        Te,
        Ti
    };


    Note(PitchClass, int, int, int);
    Note();

    PitchClass pitch_class() const;
    int octave() const;
    int on() const;
    int off() const;
    int length() const;
    bool operator<(const Note&) const;

private: 
    PitchClass p_;
    int octave_;
    int on_;
    int off_;
}; 
#endif /* NOTE_H */
