#include <cmath>
#include <algorithm>
#include <utility>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Instrument.h"

Packet Instrument::get_samples(const offset_t start_offset, const offset_t end_offset) {
    std::vector<Note> notes_to_get;
    for(Note note : notes_) {
        if(note.off() > start_offset && note.on() < end_offset) { 
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

offset_t Instrument::stream_end() const {
    return notes_.back().off(); 
}

Sample Instrument::round(double t) {
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
    const Packet::size_type start_index = 0;
    const Packet::size_type end_index = packet.size();
    Packet note_packet = cache.at(note); 
    auto on = note.on(start_offset);
    auto off = note.off(start_offset);
    on = std::max(static_cast<Packet::size_type>(on),start_index);
    off = std::min(static_cast<Packet::size_type>(off),end_index);
    for(int i = 0; i < off; ++i) {
        packet.at(i) = note_packet.at(i); 
    }
}

void Instrument::do_cache(const Note& note) {
    if (cache.find(note) == cache.end()) {
        cache[note]=gen(note);
        cache.at(note);
    }
}



