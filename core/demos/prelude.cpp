#include <cmath>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "../VorbisPlayer.h"
#include "../Instrument.h"
#include "../WaveSynth.h"

int main(int argc, char ** argv)
{
    auto p = std::make_shared<VorbisPlayer>("prelude_cpp.ogg");
    auto ws = std::make_shared<WaveSynth>(p->sample_rate(),p->freq_reference(),"wavesynth_dump.txt");
    Beat note_length(1,2);
    Sample note_intensity = Sample::max_intensity;

    WaveSynth::Notes notes;
    int note_count=0;
    for(int i = 0; i < 4; ++i) {
        for(int j : {0,2,4,7}) {
            notes.push_back(Note(j,i,note_intensity,note_length*note_count++,note_length));
        }
    }

    for(int i = 3; i >= 0; --i) {
        for(int j : {12,7,4,2}) {
            notes.push_back(Note(j,i,note_intensity,note_length*note_count++,note_length));
        }
    }

    ws->add_notes(notes);
    p->add_instrument(ws);
    p->play();
}
