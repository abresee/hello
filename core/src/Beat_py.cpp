#include <boost/python.hpp>
#include "Beat.h"
using namespace boost::python;
void export_Beat() {
    class_<Beat>("Beat",init<Beat>())
        .def(init<int64_t,int64_t>())
        .def(self * int())
        .def(int() * self)
        .def(self == self)
        .def(self < self)
        .def(self > self)
        .def(self / self)
        .def(self - self)
        .def(self + self)
        .def(self * Beat::underlying())
        .def(Beat::underlying() * self)
        .def(self / Beat::underlying())
        .def("as_double", &Beat::as_double);

}
