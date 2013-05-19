#include "Player.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;
    class_<LocalPlayer>("LocalPlayer")
        .def("play",&Player::play); 
}
