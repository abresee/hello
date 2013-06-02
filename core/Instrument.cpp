#include <cmath>
#include <algorithm>
#include <utility>
#include <iostream>
#include <boost/numeric/conversion/converter.hpp>
#include "Config.h"
#include "Instrument.h"

Packet Instrument::get_samples(const guint64 start_offset, const guint64 end_offset) {
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


void Instrument::add_notes(const Notes& notes) {
    for(Note note : notes) {
        add_note(note);    
    }
}

void Instrument::do_cache(const Note& note) {
    if (cache.find(note) == cache.end()) {
        cache[note]=gen(note);
        cache.at(note);
    }
}

double Instrument::omega(const Note n) const {
    return  2 * M_PI * frequency(n) / Config::sample_rate;
}

double Instrument::frequency(const Note n) const {
    auto exponent = (Config::pc_count*n.octave()+n.pitch_class())/static_cast<double>(Config::pc_count);
    return Config::freq_reference * pow(2,exponent);
}

guint64 Instrument::stream_end() const {
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

void Instrument::render_note(
    Packet& packet,
    const Note& note,
    const guint64 start_offset) {
    const guint64 end_offset = start_offset + packet.size();
    Packet note_packet = cache.at(note); 
    auto start = (note.on() < start_offset) ? start_offset : note.on();
    auto end = (note.off() > end_offset) ? end_offset : note.off();

    start -= start_offset;
    end -= start_offset;

    auto start_src_iter = note_packet.begin() + start;
    auto end_src_iter = note_packet.begin() + end; 
    auto start_dest_iter = packet.begin() + start;
    std::transform(
        start_src_iter,end_src_iter,
        start_dest_iter,start_dest_iter,
        std::plus<Sample>());
}
