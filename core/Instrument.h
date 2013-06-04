#ifndef INSTRUMENT_H
#define INSTRUMENT_H 
#include <unordered_map>
#include <vector>
#include <tuple>
#include <mutex>
#include <fstream>
#include <string>
#include "Packet.h"
#include "Config.h"
#include "Note.h"
/// @brief pure virtual base class for instruments
class Instrument {

public:
    typedef void (Instrument::* void_Note)(const Note&);
    typedef std::vector<Note> Notes;
    Instrument();
    Instrument(std::string dumpname);
    Packet get_samples(const offset_t start_offset, const offset_t end_offset); 
    virtual ~Instrument(){}
    void add_note(const Note& note);
    void add_notes(const Notes& notes);
    
    double omega(const Note note) const;
    double frequency(const Note note) const;
    offset_t stream_end() const;

protected:
    std::unordered_map<Note, Packet, Note_hash, Note_hash_comp> cache;

    virtual Sample round(double t);
    virtual Packet gen(const Note& note)=0;

    void render_note(Packet& packet,const Note& note,const offset_t start_offset);
    void do_cache(const Note& note);

    //utility functions
    template<typename Iter>
        static std::tuple<Iter,Iter> 
            find_zero_crossing(std::tuple<Iter,Iter> iters) {
        const auto start = std::get<0>(iters);
        const auto end = std::get<1>(iters);

        auto is_negative = [] (Iter it) -> bool {return ((*it) < 0);};
        bool first_was_negative = is_negative(start);
        auto it = start;

        for(; it != end; ++it) {
            if((is_negative(it) != first_was_negative) || 
               ((*it) == 0)) {
                break;
            }
        } 
        std::cout<<"zc found at "<<it-start<<std::endl;
        return std::make_tuple(start,it);
    }

    template<typename Iter>
    static std::tuple<Iter,Iter> silence(const std::tuple<Iter,Iter> iters) {
        auto it = std::get<0>(iters);
        const auto end = std::get<1>(iters);
        std::cout<<"filling "<<end-it<<" values with 0"<<std::endl;
        std::fill(it,end,0);
        return std::make_tuple(it,end);
    }

private:
    std::vector<Note> notes_;
    std::ofstream dump_;

};

typedef std::shared_ptr<Instrument> InstrumentHandle;
#endif /* INSTRUMENT_H */
