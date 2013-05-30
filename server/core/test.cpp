#include <cmath>
#include <iostream>
#include <stdexcept>
#include "Player.h"
#include "Instrument.h"
#include "WaveGenerator.h"
using std::cout;
using std::endl;

int main(int argc, char ** argv)
{
    std::shared_ptr<Player> p(new LocalPlayer());
    std::function<double(double)> sin= static_cast<double(*)(double)>(std::sin);
    InstrumentHandle wg = InstrumentHandle(new WaveGenerator(sin));
    std::vector<Note> notes;

    guint64 note_length = Config::sample_rate/4;

    int note_count = 0;
    for(int i = 0; i < 4; ++i)
    {
        notes.push_back(Note(0,Config::max_volume/4,i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(2,Config::max_volume/4,i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(4,Config::max_volume/4,i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(7,Config::max_volume/4,i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
    }

    for(int i = 0; i < 4; ++i)
    {
        notes.push_back(Note(0,Config::max_volume/4,4-i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(7,Config::max_volume/4,3-i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(4,Config::max_volume/4,3-i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
        notes.push_back(Note(2,Config::max_volume/4,3-i,note_length*note_count,note_length*(note_count+1)));
        note_count++;
    }

    wg->add_notes(notes);
    p->add_instrument(wg);
    p->play();
}
