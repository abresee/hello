#include <boost/python.hpp>
#include "VorbisPlayer.h"
using namespace boost::python;
void export_VorbisPlayer() {
    class_<
        VorbisPlayer,
        std::shared_ptr<VorbisPlayer>,
        bases<Player>,
        boost::noncopyable
        >(
            "VorbisPlayer",
            "saves output to a vorbis file",
            init<std::string>()
        )
        .def("play",&VorbisPlayer::play);

    implicitly_convertible<std::shared_ptr<VorbisPlayer>,PlayerHandle>();
}
