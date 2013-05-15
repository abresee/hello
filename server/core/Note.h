#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
class Note 
{
public:
    enum class PitchClass 
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

    PitchClass get_pitch_class() const;
    int get_octave() const;
    int get_start() const;
    int get_end() const;
    GstClockTime get_begin();

private: 
    PitchClass p;
    int octave;
    int start;
    int end;
};
#endif /* NOTE_H */
