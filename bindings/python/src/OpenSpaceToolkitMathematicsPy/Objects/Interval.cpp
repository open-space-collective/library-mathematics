////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Mathematics
/// @file           bindings/python/src/OpenSpaceToolkitMathematicsPy/Objects/Interval.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Core/Types/Real.hpp>

#include <OpenSpaceToolkit/Mathematics/Objects/Interval.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitMathematicsPy_Objects_Interval (        pybind11::module&           aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::Real ;

    using ostk::math::obj::Interval ;

    class_<Interval<Real>> real_interval(aModule, "RealInterval", pybind11::module_local()) ;

    // Define constructor
    real_interval.def(init<const Real&, const Real&, const Interval<Real>::Type&>())

        // Define methods
        .def(self == self)
        .def(self != self)

        .def("__str__", &(shiftToString<Interval<Real>>))
        .def("__repr__", &(shiftToString<Interval<Real>>))

        .def("is_defined", &Interval<Real>::isDefined)
        .def("is_degenerate", &Interval<Real>::isDegenerate)
        .def("intersects", &Interval<Real>::intersects)
        .def("contains_real", +[] (const Interval<Real>& anInterval, const Real& aReal) -> bool { return anInterval.contains(aReal) ; })
        .def("contains_interval", +[] (const Interval<Real>& anInterval, const Interval<Real>& anOtherInterval) -> bool { return anInterval.contains(anOtherInterval) ; })

        .def("get_lower_bound", &Interval<Real>::getLowerBound)
        .def("get_upper_bound", &Interval<Real>::getUpperBound)
        .def("to_string", &Interval<Real>::toString)

        // Define static methods
        .def_static("undefined", &Interval<Real>::Undefined)
        .def_static("closed", &Interval<Real>::Closed)
    ;

    // Add other interval types
    // ...

    // Define emuneration type for "real_interval"
    enum_<Interval<Real>::Type>(real_interval, "Type", pybind11::module_local())

        .value("Undefined", Interval<Real>::Type::Undefined)
        .value("Closed", Interval<Real>::Type::Closed)
        .value("Open", Interval<Real>::Type::Open)
        .value("HalfOpenLeft", Interval<Real>::Type::HalfOpenLeft)
        .value("HalfOpenRight", Interval<Real>::Type::HalfOpenRight)

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
