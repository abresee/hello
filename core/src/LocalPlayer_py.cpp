#include <boost/python.hpp>
#include "LocalPlayer.h"

using namespace boost::python;

void export_LocalPlayer() {
    class_<
        LocalPlayer,
        std::shared_ptr<LocalPlayer>,
        bases<Player>,
        boost::noncopyable
            >("LocalPlayer",
            "Plays back locally (i.e out the local machine's speakers)")
        .def("play",&LocalPlayer::play); 
}
