#include <boost/python.hpp>
#include "global.h"

void export_Packet();
void export_Player();
void export_LocalPlayer();
void export_VorbisPlayer();
void export_Instrument();
void export_WaveSynth();
void export_Note();
void export_Sample();
void export_Time();
void export_Offset();
void export_Beat();
void export_BadFlowException();

BOOST_PYTHON_MODULE(note_core)
{
    export_Packet();
    export_Player();
    export_LocalPlayer();
    export_VorbisPlayer();
    export_Instrument();
    export_WaveSynth();
    export_Note();
    export_Sample();
    export_Time();
    export_Offset();
    export_Beat();
    export_BadFlowException();
}
