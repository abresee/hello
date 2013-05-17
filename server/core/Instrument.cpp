#include "Instrument.h"

Instrument::~Instrument(){}
void Instrument::add_note(spNote& note)
{
    notes.push_back(note);
}
