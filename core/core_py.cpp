#include <boost/python.hpp>
#include "Note.h"
#include "Instrument.h"
#include "WaveGenerator.h"
#include "Player.h"

template<typename T>
struct std_vector_from_homogenous_python_list {
    static void* convertible(PyObject* obj_ptr) {
        return nullptr;
    }
};

BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;
    scope().attr("sample_rate") = Config::sample_rate;
    scope().attr("max_intensity") = Config::max_intensity;

    class_<Note>("Note", init<int,Sample,int,int,int>())
        .add_property("pitch_class", &Note::pitch_class)
        .add_property("intensity", &Note::intensity)
        .add_property("octave", &Note::octave)
        .add_property("on",&Note::on)
        .add_property("off",&Note::off)
        .add_property("length",&Note::length);

    class_<Instrument,std::shared_ptr<Instrument>,boost::noncopyable>("Instrument", "Abstract base class",no_init)
        .def("add_note",&Instrument::add_note)
        .def("add_notes",&Instrument::add_notes);

    class_<WaveGenerator,std::shared_ptr<WaveGenerator>,bases<Instrument>,boost::noncopyable>("WaveGenerator",
            "creates a signal based on a waveform");

    class_<Player,std::shared_ptr<Player>, boost::noncopyable>("Player", "Abstract base class", no_init)
        .def("play",&Player::play)
        .def("add_instrument",&Player::add_instrument);

    class_<LocalPlayer,std::shared_ptr<LocalPlayer>,bases<Player>,boost::noncopyable>("LocalPlayer",
            "Plays back locally (i.e out the local machine's speakers)")
        .def("play",&LocalPlayer::play); 

    implicitly_convertible<std::shared_ptr<WaveGenerator>,InstrumentHandle>();
}
