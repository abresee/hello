#include <cmath>
#include <iostream>
#include <stdexcept>
#include "../Player.h"
#include "../Instrument.h"
#include "../WaveSynth.h"

int main(int argc, char ** argv)
{
    std::shared_ptr<Player> p(new LocalPlayer());
    InstrumentHandle wg = InstrumentHandle(new WaveSynth("data"));
    std::vector<Note> notes;

    auto note_length = Config::sample_rate/4;
    std::cout<<"each note is "<<note_length<<" samples long"<<std::endl;
    Sample note_intensity = Config::max_intensity/4;

    int note_count=0;
    for(int i = 0; i < 4; ++i) {
        for(int j : {0,2,4,7}) {
            notes.push_back(Note(j,i,note_intensity,note_length*note_count,note_length));
            note_count++;
        }
    }

    for(int i = 3; i >= 0; --i) {
        for(int j : {12,7,4,2}) {
            notes.push_back(Note(j,i,note_intensity,note_length*note_count,note_length));
            note_count++;
        }
    }

    wg->add_notes(notes);
    p->add_instrument(wg);
    p->play();
    std::cout<<std::endl;
}
