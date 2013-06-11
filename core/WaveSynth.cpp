#include <cmath>
#include <numeric>
#include <iostream>
#include "WaveSynth.h"

//////////////////////////////
// Wave Inner Class Definition
//////////////////////////////
WaveSynth::Wave::Wave():
    functor_(static_cast<double(*)(double)>(std::sin)){
}
WaveSynth::Wave::Wave(const Functor& functor_initializer):
    functor_(functor_initializer) {
}

double WaveSynth::Wave::operator()(const double angle) const {
    auto result = functor_(angle);
    return result;
}

///////////////////////////////
// Waves Inner Class Definition
///////////////////////////////
WaveSynth::Waves::Waves(const size_type size):
    waves_(size,Wave()) {
}
WaveSynth::Waves::Waves(const size_type size,const Wave& wave_initializer):
    waves_(size,wave_initializer) {
}
WaveSynth::Waves::Waves(std::initializer_list<Wave> l):
    waves_(l) {
}

double WaveSynth::Waves::operator()(const double angle) const {
    double result=0;
        for(Wave wave : waves_) {
            result+=wave(angle);
        }
    return result;
}

/////////////////////////////////
// WaveSynth Class Definition
/////////////////////////////////
WaveSynth::WaveSynth():
    waves_(1) { //constructs the "waves_" to have 1 default constructed value ie std::sin)
} 
WaveSynth::WaveSynth(std::string s): Instrument(s), waves_(1){}
WaveSynth::WaveSynth(const Wave& wave_initializer):
    waves_(1,wave_initializer) {
}
WaveSynth::WaveSynth(Wave&& wave_initializer): 
    waves_(1,std::move(wave_initializer)) {
}
WaveSynth::WaveSynth(const Waves& waves_initializer):
    waves_(waves_initializer) {
}
WaveSynth::WaveSynth(Waves&& waves_initializer):
    waves_(std::move(waves_initializer)) {
}
WaveSynth::WaveSynth(std::initializer_list<Wave> l):
    waves_(l) {
}

Packet WaveSynth::gen(const Note& note) {
    const Offset length = note.length().to_offset(Config::tempo, Config::sample_rate);
    Packet ret(length.value());
    for(int i = 0; i < length.value(); ++i) {
        ret.at(i)=round(note.intensity().value()*waves_(omega(note) * (i)));
    }
    //const auto fade_len(3*period_i(note).value());
    //fade(std::make_tuple(ret.begin(),ret.begin()+fade_len));
    //fade(std::make_tuple(ret.rbegin(),ret.rbegin()+fade_len));
    return ret;
}
