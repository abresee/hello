#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <vector>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument
{
public:
    PacketHandle get_samples(int begin,int end); 
    virtual ~Instrument();
    virtual void add_note(Note&);
    virtual double omega(const Note&) const;
    virtual double frequency(const Note&) const;
protected:
    virtual void gen(Note&,Packet&,int,int)=0;
    template<typename T>
    Sample round(T);

private:
    void generate(Note&, Packet&, int);
    std::vector<Note> notes;
};
#endif /* INSTRUMENT_H */
