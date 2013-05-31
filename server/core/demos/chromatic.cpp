#include <cmath>
#include <iostream>
#include <stdexcept>
#include "../Player.h"
#include "../Instrument.h"
#include "../WaveGenerator.h"
using std::cout;
using std::endl;

int main(int argc, char ** argv)
{
    std::shared_ptr<Player> p(new LocalPlayer());
    InstrumentHandle wg = InstrumentHandle(new WaveGenerator());
    std::vector<Note> notes;

    guint64 note_length = Config::sample_rate/8;
    Sample note_intensity = Config::max_intensity/4;

    for(int i = 0; i < 140; ++i)
    {
        notes.push_back(Note(i,-5,note_intensity,note_length*i,note_length*(i+1)));
    }

    wg->add_notes(notes);
    p->add_instrument(wg);
    p->play();
}
