#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include <functional>
#include <tuple>
#include "global.h"



class Note {
public:
    Note(int pc_init, int octave_init, Sample intensity_init, position_t pos_init, position_t len_init);
    
    typedef int (Note::* int_void_const)() const;
    typedef position_t (Note::* position_void_const)() const;
    

    int pitch_class() const;
    int pitch_class(int ref) const;

    int octave() const;
    int octave(int ref) const;

    position_t position() const;
    position_t position(position_t ref) const;
    position_t end() const;
    position_t end(position_t red) const;

    //note length is consistent in all reference frames, so no overload
    position_t length() const;
    //no such thing as frame of reference for "intensity" -- the scale is by definition absolute, so no need to overload this one either
    Sample intensity() const;

    bool operator<(const Note&) const;
    bool operator==(const Note&) const;

private: 
    int pitch_class_;
    int octave_;

    Sample intensity_;

    position_t pos_;
    position_t length_;
}; 

struct Note_hash_comp{
    bool operator()(const Note& lhs, const Note& rhs) const noexcept{
        return 
            (lhs.pitch_class() == rhs.pitch_class()) &&
            (lhs.octave() == rhs.octave()) &&
            (lhs.intensity() == rhs.intensity()) &&
            (lhs.length() == rhs.length());
    }
};

struct Note_hash {
    size_t operator()(const Note& note) const {
            return boost::hash_value(std::make_tuple(
                note.pitch_class(),note.octave(),note.intensity(),boost::rational_cast<double>(note.length())));
    }
};
#endif /* NOTE_H */
