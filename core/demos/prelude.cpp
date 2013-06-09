#include <cmath>
#include <memory>
#include <iostream>
#include <stdexcept>
#include "../Player.h"
#include "../Instrument.h"
#include "../WaveSynth.h"

int main(int argc, char ** argv)
{
    auto p = std::make_shared<VorbisPlayer>("test.ogg");
    auto wg = std::make_shared<WaveSynth>("data");
    auto note_length = Config::sample_rate/4;
    Sample note_intensity = Config::max_intensity/4;

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

    wg->add_notes(notes);
    p->add_instrument(wg);
    p->play();
}
