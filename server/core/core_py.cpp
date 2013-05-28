#include "Player.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;

    class_<Player,boost::noncopyable>("Player",
            "Abstract base class",
            no_init)
        .def("play",&Player::play);

    class_<LocalPlayer,bases<Player>,boost::noncopyable>("LocalPlayer",
            "Plays back locally (i.e out the local machine's speakers)")
        .def("play",&LocalPlayer::play); 
}
