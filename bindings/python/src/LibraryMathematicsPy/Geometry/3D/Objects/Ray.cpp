////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Library ▸ Mathematics
/// @file           LibraryMathematicsPy/Geometry/3D/Objects/Ray.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Library/Mathematics/Geometry/3D/Objects/Ray.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     LibraryMathematicsPy_Geometry_3D_Objects_Ray ( )
{

    using namespace boost::python ;

    using library::math::obj::Vector3d ;
    using library::math::geom::d3::Object ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::PointSet ;
    using library::math::geom::d3::objects::Ray ;
    using library::math::geom::d3::objects::Plane ;
    using library::math::geom::d3::objects::Sphere ;
    using library::math::geom::d3::objects::Ellipsoid ;
    using library::math::geom::d3::Intersection ;

    scope in_Ray = class_<Ray, bases<Object>>("Ray", init<const Point&, const Vector3d&>())

        .def(self == self)
        .def(self != self)

        .def(self_ns::str(self_ns::self))
        .def(self_ns::repr(self_ns::self))

        .def("isDefined", &Ray::isDefined)
        .def("intersectsPoint", +[] (const Ray& aRay, const Point& aPoint) -> bool { return aRay.intersects(aPoint) ; })
        .def("intersectsPlane", +[] (const Ray& aRay, const Plane& aPlane) -> bool { return aRay.intersects(aPlane) ; })
        .def("intersectsSphere", +[] (const Ray& aRay, const Sphere& aSphere) -> bool { return aRay.intersects(aSphere) ; })
        .def("intersectsEllipsoid", +[] (const Ray& aRay, const Ellipsoid& anEllipsoid) -> bool { return aRay.intersects(anEllipsoid) ; })
        .def("containsPoint", +[] (const Ray& aRay, const Point& aPoint) -> bool { return aRay.contains(aPoint) ; })
        .def("containsPointSet", +[] (const Ray& aRay, const PointSet& aPointSet) -> bool { return aRay.contains(aPointSet) ; })

        .def("getOrigin", &Ray::getOrigin)
        .def("getDirection", &Ray::getDirection)
        .def("intersectionWithPlane", +[] (const Ray& aRay, const Plane& aPlane) -> Intersection { return aRay.intersectionWith(aPlane) ; })
        .def("intersectionWithEllipsoid", +[] (const Ray& aRay, const Ellipsoid& anEllipsoid) -> Intersection { return aRay.intersectionWith(anEllipsoid) ; })
        .def("applyTransformation", &Ray::applyTransformation)

        .def("Undefined", &Ray::Undefined).staticmethod("Undefined")

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
