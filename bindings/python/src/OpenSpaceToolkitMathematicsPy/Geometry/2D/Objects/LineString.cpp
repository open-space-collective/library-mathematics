////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Mathematics
/// @file           bindings/python/src/OpenSpaceToolkitMathematicsPy/Geometry/2D/Objects/LineString.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Mathematics/Geometry/2D/Objects/LineString.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (OpenSpaceToolkitMathematicsPy_Geometry_2D_Objects_LineString_toString_overloads, ostk::math::geom::d2::objects::LineString::toString, 0, 2)

inline void                     OpenSpaceToolkitMathematicsPy_Geometry_2D_Objects_LineString ( )
{

    using namespace boost::python ;

    using ostk::core::ctnr::Array ;

    using ostk::math::geom::d2::Object ;
    using ostk::math::geom::d2::objects::Point ;
    using ostk::math::geom::d2::objects::LineString ;

    scope in_LineString = class_<LineString, bases<Object>>("LineString", init<const Array<Point>&>())

        .def(self == self)
        .def(self != self)

        .def(self_ns::str(self_ns::self))
        .def(self_ns::repr(self_ns::self))

        .def("isDefined", &LineString::isDefined)
        .def("isEmpty", &LineString::isEmpty)
        .def("isNear", &LineString::isNear)

        .def("getPointCount", &LineString::getPointCount)
        .def("getPointClosestTo", &LineString::getPointClosestTo)
        .def("toString", &LineString::toString, OpenSpaceToolkitMathematicsPy_Geometry_2D_Objects_LineString_toString_overloads())
        .def("applyTransformation", &LineString::applyTransformation)

        .def("Empty", &LineString::Empty).staticmethod("Empty")

        .def("__len__", &LineString::getPointCount)
        .def("__getitem__", +[] (const LineString& aLineString, const size_t anIndex) -> const Point& { return aLineString.accessPointAt(anIndex) ; }, return_internal_reference<>())
        .def("__iter__", range(&LineString::begin, &LineString::end))

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
