////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Mathematics
/// @file           bindings/python/src/OpenSpaceToolkitMathematicsPy/Geometry/3D/Objects/Composite.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Intersection.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/Composite.hpp>

#include <OpenSpaceToolkit/Core/Types/Shared.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void                     OpenSpaceToolkitMathematicsPy_Geometry_3D_Objects_Composite ( pybind11::module&     aModule                                     )
{

    using namespace pybind11 ;

    using ostk::core::types::Shared ;
    using ostk::core::types::Real ;

    using ostk::math::obj::Vector3d ;
    using ostk::math::geom::d3::Object ;
    using ostk::math::geom::d3::objects::Point ;
    using ostk::math::geom::d3::objects::PointSet ;
    using ostk::math::geom::d3::objects::Line ;
    using ostk::math::geom::d3::objects::Ray ;
    using ostk::math::geom::d3::objects::Segment ;
    using ostk::math::geom::d3::objects::LineString ;
    using ostk::math::geom::d3::objects::Polygon ;
    using ostk::math::geom::d3::objects::Plane ;
    using ostk::math::geom::d3::objects::Sphere ;
    using ostk::math::geom::d3::objects::Ellipsoid ;
    using ostk::math::geom::d3::objects::Cuboid ;
    using ostk::math::geom::d3::objects::Pyramid ;
    using ostk::math::geom::d3::objects::Composite ;
    using ostk::math::geom::d3::Intersection ;
    using ostk::math::geom::d3::trf::rot::Quaternion ;

    class_<Composite, Object>(aModule, "Composite")

        .def(init<const Object&>())

        .def(self == self)
        .def(self == self)

        .def(self + self)
        .def(self += self)

        .def("__str__", &(shiftToString<Composite>))
        .def("__repr__", &(shiftToString<Composite>))

        .def("is_defined", &Composite::isDefined)
        .def("is_empty", &Composite::isEmpty)

        .def("is_point", +[] (const Composite& aComposite) -> bool { return aComposite.is<Point>() ; })
        .def("is_point_set", +[] (const Composite& aComposite) -> bool { return aComposite.is<PointSet>() ; })
        .def("is_line", +[] (const Composite& aComposite) -> bool { return aComposite.is<Line>() ; })
        .def("is_ray", +[] (const Composite& aComposite) -> bool { return aComposite.is<Ray>() ; })
        .def("is_segment", +[] (const Composite& aComposite) -> bool { return aComposite.is<Segment>() ; })
        .def("is_line_string", +[] (const Composite& aComposite) -> bool { return aComposite.is<LineString>() ; })
        .def("is_polygon", +[] (const Composite& aComposite) -> bool { return aComposite.is<Polygon>() ; })
        .def("is_plane", +[] (const Composite& aComposite) -> bool { return aComposite.is<Plane>() ; })
        .def("is_sphere", +[] (const Composite& aComposite) -> bool { return aComposite.is<Sphere>() ; })
        .def("is_ellipsoid", +[] (const Composite& aComposite) -> bool { return aComposite.is<Ellipsoid>() ; })
        .def("is_pyramid", +[] (const Composite& aComposite) -> bool { return aComposite.is<Pyramid>() ; })
        .def("is_composite", +[] (const Composite& aComposite) -> bool { return aComposite.is<Composite>() ; })

        .def("intersects_object", +[] (const Composite& aComposite, const Object& anObject) -> bool { return aComposite.intersects(anObject) ; })
        .def("intersects_composite", +[] (const Composite& aComposite, const Composite& anotherComposite) -> bool { return aComposite.intersects(anotherComposite) ; })
        .def("contains_object", +[] (const Composite& aComposite, const Object& anObject) -> bool { return aComposite.contains(anObject) ; })
        .def("contains_composite", +[] (const Composite& aComposite, const Composite& anotherComposite) -> bool { return aComposite.contains(anotherComposite) ; })

        .def("as_point", +[] (const Composite& aComposite) -> Point { return aComposite.as<Point>() ; })
        .def("as_point_set", +[] (const Composite& aComposite) -> PointSet { return aComposite.as<PointSet>() ; })
        .def("as_line", +[] (const Composite& aComposite) -> Line { return aComposite.as<Line>() ; })
        .def("as_ray", +[] (const Composite& aComposite) -> Ray { return aComposite.as<Ray>() ; })
        .def("as_segment", +[] (const Composite& aComposite) -> Segment { return aComposite.as<Segment>() ; })
        .def("as_line_string", +[] (const Composite& aComposite) -> LineString { return aComposite.as<LineString>() ; })
        .def("as_polygon", +[] (const Composite& aComposite) -> Polygon { return aComposite.as<Polygon>() ; })
        .def("as_plane", +[] (const Composite& aComposite) -> Plane { return aComposite.as<Plane>() ; })
        .def("as_sphere", +[] (const Composite& aComposite) -> Sphere { return aComposite.as<Sphere>() ; })
        .def("as_ellipsoid", +[] (const Composite& aComposite) -> Ellipsoid { return aComposite.as<Ellipsoid>() ; })
        .def("as_pyramid", +[] (const Composite& aComposite) -> Pyramid { return aComposite.as<Pyramid>() ; })
        .def("as_composite", +[] (const Composite& aComposite) -> Composite { return aComposite.as<Composite>() ; })

        // Casting issue with both boost and pybind11 (likely related to pointers' handling)
        // .def("access_object_at", &Composite::accessObjectAt, return_value_policy<reference_existing_object>())
        // .def("access_objects", &Composite::accessObjects, return_value_policy<reference_existing_object>())
        .def("access_object_at", &Composite::accessObjectAt, return_value_policy::reference)
        // .def("access_objects", &Composite::accessObjects, return_value_policy::reference) // https://stackoverflow.com/questions/53807248/pybind11-returning-a-pointer-to-a-container-of-unique-ptr
        .def("get_object_count", &Composite::getObjectCount)
        .def("intersection_with_object", +[] (const Composite& aComposite, const Object& anObject) -> Intersection { return aComposite.intersectionWith(anObject) ; })
        .def("intersection_with_composite", +[] (const Composite& aComposite, const Composite& anotherComposite) -> Intersection { return aComposite.intersectionWith(anotherComposite) ; })

        .def("apply_transformation", &Composite::applyTransformation)

        .def_static("undefined", &Composite::Undefined)
        .def_static("empty", &Composite::Empty)

    ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
