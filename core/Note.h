#ifndef NOTE_H
#define NOTE_H 
#include <gst/gst.h>
#include <functional>
#include <boost/functional/hash.hpp>
#include <tuple>
#include "Config.h"


class Note 
{
public:
    Note(int pc_init, int octave_init, Sample intensity_init, offset_t on_init, offset_t off_init);
    Note(int pc_init, int octave_init, Sample intensity_init, offset_t on_init, offset_t off_init , offset_t ref_init);
    
    typedef int (Note::* int_void_const)() const;
    typedef offset_t (Note::* offset_void_const)() const;
    int pitch_class() const;
    int pitch_class(int ref) const;

    int octave() const;
    int octave(int ref) const;

    offset_t on() const;
    offset_t on(offset_t ref) const;

    offset_t off() const;
    offset_t off(offset_t ref) const;
    
    //unless we're moving super fast, note length is consistent in all reference frames, so no overload
    offset_t length() const;
    //no such thing as frame of reference for "intensity" -- the scale is by definition absolute, so no need to overload this one either
    Sample intensity() const;

    offset_t ref() const;
    bool operator<(const Note&) const;
    bool operator==(const Note&) const;

private: 
    int pitch_class_abs() const;
    int pitch_class_;
    int octave_;

    Sample intensity_;

    offset_t on_;
    offset_t off_;
    offset_t ref_;
}; 

struct hash_comp{
    bool operator()(const Note& lhs, const Note& rhs) const noexcept{
        return 
            (lhs.pitch_class() == rhs.pitch_class()) &&
            (lhs.octave() == rhs.octave()) &&
            (lhs.intensity() == rhs.intensity()) &&
            (lhs.length() == rhs.length());
    }
};


namespace std {
    template<typename... T>
    struct hash<tuple<T...>>{
        size_t operator()(tuple<T...> const& arg) const noexcept
        {
            return boost::hash_value(arg);
        }
    };

    template<>
    struct hash<Note> {
        size_t operator()(const Note& note) const {
            return std::hash<std::tuple<int,int,Sample,offset_t>>()(
                std::make_tuple(
                    note.pitch_class(),note.octave(),note.intensity(),note.length()));
        }
    };
}
#endif /* NOTE_H */
