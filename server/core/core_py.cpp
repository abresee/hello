#include "player.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(core_py)
{
    using namespace boost::python;
    class_<Player>("Player")
        .def("play",&Player::play); 
}
