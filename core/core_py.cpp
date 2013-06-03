#include <boost/python.hpp>
#include "Note.h"
#include "Instrument.h"
#include "WaveSynth.h"
#include "Player.h"

Note::int_void_const pc = &Note::pitch_class;
Note::int_void_const oct = &Note::octave;
Note::offset_void_const pos = &Note::position;
Note::offset_void_const off_ptr= &Note::off;
Instrument::void_Note add_note = &Instrument::add_note;

BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;
    scope().attr("sample_rate") = Config::sample_rate;
    scope().attr("max_intensity") = Config::max_intensity;


    class_<Note>("Note", init<int,Sample,int,int,int>())
        .add_property("pitch_class",pc)
        .add_property("octave", oct)
        .add_property("intensity", &Note::intensity)
        .add_property("position",pos)
        .add_property("off",off_ptr)
        .add_property("length",&Note::length);

    class_<Instrument,std::shared_ptr<Instrument>,boost::noncopyable>("Instrument", "Abstract base class",no_init)
        .def("add_note",add_note)
        .def("add_notes",&Instrument::add_notes);

    class_<WaveSynth,std::shared_ptr<WaveSynth>,bases<Instrument>,boost::noncopyable>("WaveSynth",
            "creates a signal based on a waveform");

    class_<Player,std::shared_ptr<Player>, boost::noncopyable>("Player", "Abstract base class", no_init)
        .def("play",&Player::play)
        .def("add_instrument",&Player::add_instrument);

    class_<LocalPlayer,std::shared_ptr<LocalPlayer>,bases<Player>,boost::noncopyable>("LocalPlayer",
            "Plays back locally (i.e out the local machine's speakers)")
        .def("play",&LocalPlayer::play); 

    implicitly_convertible<std::shared_ptr<WaveSynth>,InstrumentHandle>();
}
