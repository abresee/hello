#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include "Player.h"

class Note 
{
public:


    Note(int,Player::Sample,int,int,int);

    int pitch_class() const;
    Player::Sample intensity() const;
    double omega() const;
    int octave() const;
    int on() const;
    int off() const;
    int length() const;
    bool operator<(const Note&) const;

private: 
    int pitch_class_;
    Player::Sample intensity_;
    int octave_;
    int on_;
    int off_;
}; 
#endif /* NOTE_H */
