#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include "Config.h"

class Note 
{
public:
    Note(int,int,Sample,guint64,guint64);

    int pitch_class() const;
    int octave() const;
    Sample intensity() const;
    guint64 on() const;
    guint64 off() const;
    guint64 length() const;
    bool operator<(const Note&) const;
    bool operator==(const Note&) const;

private: 
    int pitch_class_;
    int octave_;
    Sample intensity_;
    int on_;
    int off_;
}; 
#endif /* NOTE_H */
