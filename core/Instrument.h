#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <unordered_map>
#include <vector>
#include <tuple>
#include "Packet.h"
#include "Config.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument {

public:
    typedef std::vector<Note> Notes;
    Instrument() : notes_() {}
    Packet get_samples(const guint64 start_offset, const guint64 end_offset); 
    virtual ~Instrument(){}
    void add_note(const Note& note);
    void add_notes(const Notes& notes);
    
    double omega(const Note note) const;
    double frequency(const Note note) const;
    guint64 stream_end() const;
protected:
    virtual 
    Sample round(double t);
    std::unordered_map<Note, Packet, std::hash<Note>,hash_comp> cache;
    virtual Packet gen(const Note& note)=0;

private:
    void render_note(
        Packet& packet, 
        const Note& note,
        const guint64 start_offset);

    void do_cache(const Note& note);

    std::vector<Note> notes_;

};
typedef std::shared_ptr<Instrument> InstrumentHandle;
#endif /* INSTRUMENT_H */
