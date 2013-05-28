#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <vector>
#include "Config.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    void get_samples(Packet& p,const guint64 begin_offset); 
    virtual ~Instrument();
    void add_note(const Note note);
    void add_notes(const std::vector<Note> notes);
    
    double omega(const Note note) const;
    double frequency(const Note note) const;
    guint64 stream_end() const;
protected:
    virtual void gen(const Note note, Packet& p, const guint64 start_offset)=0;
    Sample round(double t);

private:
    void generate(const Note note, Packet& p, const guint64 start_offset);
    std::vector<Note> notes;
};
#endif /* INSTRUMENT_H */
