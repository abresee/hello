#include <boost/python.hpp>
#include "Offset.h"

using namespace boost::python;

void export_Offset() {
    class_<Offset>("Offset",init<Offset::underlying>())
        .def("value",&Offset::value)
        .def("as_double",&Offset::as_double)
        .def(self < self)
        .def(self <= self)
        .def(self > self)
        .def(self >= self)
        .def(self == self)
        .def(self - self)
        .def(self + self)
        .def(self + Offset::underlying())
        .def(self += Offset::underlying())
        .def(self * Offset::underlying());
}
