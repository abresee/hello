#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include "Config.h"

class Note 
{
public:
    Note(int,Sample,int,guint64,guint64);

    int pitch_class() const;
    Sample intensity() const;
    int octave() const;
    guint64 on() const;
    guint64 off() const;
    guint64 length() const;
    bool operator<(const Note&) const;

private: 
    int pitch_class_;
    Sample intensity_;
    int octave_;
    int on_;
    int off_;
}; 
#endif /* NOTE_H */
