#include <iostream>
#include "Note.h"

Note::Note(int pc_init, int octave_init, Sample intensity_init, offset_t on_init, offset_t off_init, offset_t ref_init):
    pitch_class_(pc_init),
    octave_(octave_init),
    intensity_(intensity_init),
    on_(on_init),
    off_(off_init),
    ref_(ref_init){

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

Note::Note(int pc_init, int octave_init, Sample intensity_init, offset_t on_init, offset_t off_init):
    Note(pc_init, octave_init, intensity_init, on_init, off_init, 0) {
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


offset_t Note::on() const {
    return on_;
}
offset_t Note::on(offset_t ref) const {
    return on() - ref; 
}

offset_t Note::off() const {
    return off_;
}
offset_t Note::off(offset_t ref) const {
    return off() - ref;
}

offset_t Note::length() const {
    return off() - on();
}

Sample Note::intensity() const {
    return intensity_;
}

bool Note::operator<(const Note& other) const {
    return off() < other.off();
}

bool Note::operator==(const Note& other) const {
    return (pitch_class_ < other.pitch_class_) &&
        (octave_ < other.octave_) &&
        (intensity_ < other.intensity_) &&
        (on_ < other.on_) &&
        (off_ < other.off_ ) ;
}
