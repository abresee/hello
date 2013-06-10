#include <cmath>
#include <algorithm>
#include <utility>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/assert.hpp>
#include "Instrument.h"

Instrument::Instrument(): notes_() {}

Instrument::Instrument(std::string dumpname): notes_(), dump_(dumpname) {}

Packet Instrument::get_samples(const offset_t start_offset, const offset_t end_offset) {
    std::vector<Note> notes_to_get;
    for(Note note : notes_) {
        if((note.position() + note.length()) > start_offset && note.position() < end_offset) { 
            notes_to_get.push_back(note);
        }
    }

    Packet ret(Packet::size_type(end_offset-start_offset)); 
    for(Note note : notes_to_get) {
        render_note(ret,note,start_offset);
    }
    return ret;
}

void Instrument::add_note(const Note& note) {
    notes_.push_back(note);
    do_cache(note);
    std::sort(notes_.begin(),notes_.end());
}

//not the most efficient way to do this but I'm trying to achieve some DRY with the caching
//TODO: make this less shitty
void Instrument::add_notes(const Notes& notes) {
    for(Note note : notes) {
        add_note(note);    
    }
}

double Instrument::omega(const Note n) const {
    return  2 * M_PI * frequency(n) / Config::sample_rate;
}

double Instrument::frequency(const Note n) const {
    auto exponent = (Config::pc_count*n.octave()+n.pitch_class())/static_cast<double>(Config::pc_count);
    return Config::freq_reference * pow(2,exponent);
}

double Instrument::period(const Note n) const {
    return 1.0/frequency(n);
}

double Instrument::rperiod(const Note n) const {
    return 1.0/omega(n);
}
//TODO: ensure this rounds consistently, create an actual converter object
offset_t Instrument::period_i(const Note n) const {
    auto ret = Config::sample_rate*period(n); 
    return static_cast<offset_t>(ret);
}

offset_t Instrument::rperiod_i(const Note n) const {
    return Config::sample_rate*rperiod(n);
}

position_t Instrument::stream_end() const {
    return notes_.back().end(); 
}

Sample Instrument::round(double t) const {
    using namespace boost::numeric;
    converter<Sample
        , double
        , conversion_traits<Sample, double>
        , def_overflow_handler
        , Trunc<typename conversion_traits<Sample, double>::source_type>
        > typeconverter;    
    return typeconverter(t); 
}

void Instrument::render_note(Packet& packet, const Note& note, const offset_t start_offset) {
    const Packet note_packet = cache.at(note); 

    const offset_t note_begin_output_index = Config::position_to_offset(note.position(),Config::tempo, Config::sample_rate) - start_offset;
    const offset_t note_end_output_index = note_begin_output_index + Config::position_to_offset(note.length(),Config::tempo, Config::sample_rate);

    const offset_t begin_output_index = 
        (note_begin_output_index < 0) ? 0 : note_begin_output_index;
    const offset_t end_output_index = 
        (note_end_output_index > packet.size()) ? packet.size() : note_end_output_index;
    BOOST_ASSERT(begin_output_index < end_output_index);

    const offset_t index_offset = note_begin_output_index;
    for(auto i = begin_output_index; i < end_output_index; ++i) {
        packet.at(i) += note_packet.at(i-index_offset); 
        dump_<<packet.at(i)<<" ";
    }
}

void Instrument::do_cache(const Note& note) {
    if (cache.find(note) == cache.end()) {
        cache[note]=gen(note);
        cache.at(note);
    }
}



