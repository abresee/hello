#include <boost/python.hpp>
#include "Player.h"

using namespace boost::python;
void export_Player() {
    class_<Player,PlayerHandle, boost::noncopyable>("Player", "Abstract base class", no_init)
        .def("play",&Player::play)
        .def("add_instrument",&Player::add_instrument)
        .def("wait_until_ready",&Player::wait_until_ready);
}
