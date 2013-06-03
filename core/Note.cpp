#include <iostream>
#include "Note.h"

Note::Note(int pc_init, int octave_init, Sample intensity_init, offset_t pos_init, offset_t len_init): pitch_class_(pc_init), octave_(octave_init), intensity_(intensity_init), pos_(pos_init), length_(len_init) {

    using Config::pc_count;
    if (pitch_class_ < 0) {
        octave_ += pitch_class_ / pc_count - 1; 
        pitch_class_=(pitch_class_ % pc_count)+pc_count;
    }

    else if (pitch_class_ > 11) {
        octave_ += pitch_class_ / pc_count;
        pitch_class_%=pc_count;
    }
}

int Note::pitch_class() const {
    return pitch_class_;
}
int Note::pitch_class(int ref) const {
    return pitch_class() - ref;
}

int Note::octave() const {
    return octave_;
}

int Note::octave(int ref) const {
    return octave() - ref;
}

offset_t Note::position() const {
    return pos_;
}
offset_t Note::position(offset_t ref) const {
    return position() - ref; 
}

offset_t Note::off() const {
    return position() + length();
}
offset_t Note::off(offset_t ref) const {
    return position(ref) + length();
}

offset_t Note::length() const {
    return length_;
}


Sample Note::intensity() const {
    return intensity_;
}


bool Note::operator<(const Note& other) const {
    if(position() < other.position()) {
        return true;
    }
    else if(position() > other.position()) {
        return false;
    }
    else {
        return (position() + length()) < (other.position()+other.length());
    }
}

bool Note::operator==(const Note& other) const {
    return (pitch_class() < other.pitch_class()) &&
        (octave() < other.octave()) &&
        (intensity() < other.intensity()) &&
        (position() < other.position()) &&
        (length() < other.length());
}
