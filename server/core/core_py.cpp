#include <boost/python.hpp>
#include "Note.h"
#include "Instrument.h"
#include "WaveGenerator.h"
#include "Player.h"
BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;

    class_<Note>("Note", init<int,Sample,int,int,int>())
        .add_property("pitch_class", &Note::pitch_class)
        .add_property("intensity", &Note::intensity)
        .add_property("octave", &Note::octave)
        .add_property("on",&Note::on)
        .add_property("off",&Note::off)
        .add_property("length",&Note::length);

    class_<Instrument,boost::noncopyable>("Instrument", "Abstract base class",no_init)
        .def("add_note",&Instrument::add_note)
        .def("add_notes",&Instrument::add_notes);

    class_<WaveGenerator,bases<Instrument>,boost::noncopyable>("WaveGenerator",
            "creates a signal based on a waveform", init<std::function<double(double)>>());
    class_<Player,boost::noncopyable>("Player", "Abstract base class", no_init)
        .def("play",&Player::play)
        .def("add_instrument",&Player::add_instrument);

    class_<LocalPlayer,bases<Player>,boost::noncopyable>("LocalPlayer",
            "Plays back locally (i.e out the local machine's speakers)")
        .def("play",&LocalPlayer::play); 
}
