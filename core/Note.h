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
            return std::hash<std::tuple<int,int,Sample,guint64>>()(
                std::make_tuple(
                    note.pitch_class(),note.octave(),note.intensity(),note.length()));
        }
    };
}
#endif /* NOTE_H */
