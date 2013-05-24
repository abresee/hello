#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <vector>
#include "Config.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    void get_samples(Packet& p, guint64 begin_offset); 
    virtual ~Instrument();
    virtual void add_note(Note&);
    virtual double omega(const Note&) const;
    virtual double frequency(const Note&) const;
    virtual guint64 stream_end() const;
protected:
    virtual void gen(Note& note, Packet& p, int on, int off)=0;
    Sample round(double t);

private:
    void generate(Note& note, Packet& p, const int start_offset);
    std::vector<Note> notes;
};
#endif /* INSTRUMENT_H */
