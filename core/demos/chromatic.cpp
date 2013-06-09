#include <cmath>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "../Player.h"
#include "../Instrument.h"
#include "../WaveSynth.h"

int main(int argc, char ** argv)
{
    PlayerHandle p = std::make_shared<LocalPlayer>();
    InstrumentHandle wg = std::make_shared<WaveSynth>("data");

    offset_t note_length = Config::sample_rate/8;
    Sample note_intensity = Config::max_intensity/4;
    WaveSynth::Notes notes;
    for(int i = 0; i < 80; ++i) {
        notes.push_back(
            Note(i,-1,note_intensity,note_length*i,note_length));
    }

    wg->add_notes(notes);
    p->add_instrument(wg);
    p->play();
}