////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Library/Mathematics
/// @file           Library/Mathematics/Geometry/3D/Objects/Polygon.test.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        TBD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Library/Mathematics/Geometry/Transformations/Rotations/RotationVector.hpp>
#include <Library/Mathematics/Geometry/3D/Objects/Polygon.hpp>

#include <Global.test.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, Constructor)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_NO_THROW(Polygon(polygon2d, origin, xAxis, yAxis)) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, Clone)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_NO_THROW(Polygon(polygon2d, origin, xAxis, yAxis).clone()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, EqualToOperator)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_TRUE(Polygon(polygon2d, origin, xAxis, yAxis) == Polygon(polygon2d, origin, xAxis, yAxis)) ;

    }

    {

        // [TBI] Implement similarities

    }

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_FALSE(Polygon(polygon2d, origin, xAxis, yAxis) == Polygon::Undefined()) ;
        EXPECT_FALSE(Polygon::Undefined() == Polygon(polygon2d, origin, xAxis, yAxis)) ;
        EXPECT_FALSE(Polygon::Undefined() == Polygon::Undefined()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, NotEqualToOperator)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_FALSE(Polygon(polygon2d, origin, xAxis, yAxis) != Polygon(polygon2d, origin, xAxis, yAxis)) ;

    }

    {

        // [TBI] Implement similarities

    }

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_TRUE(Polygon(polygon2d, origin, xAxis, yAxis) != Polygon::Undefined()) ;
        EXPECT_TRUE(Polygon::Undefined() != Polygon(polygon2d, origin, xAxis, yAxis)) ;
        EXPECT_TRUE(Polygon::Undefined() != Polygon::Undefined()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, StreamOperator)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        testing::internal::CaptureStdout() ;

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_NO_THROW(std::cout << Polygon(polygon2d, origin, xAxis, yAxis) << std::endl) ;

        EXPECT_FALSE(testing::internal::GetCapturedStdout().empty()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, IsDefined)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_TRUE(Polygon(polygon2d, origin, xAxis, yAxis).isDefined()) ;

    }

    {

        EXPECT_FALSE(Polygon::Undefined().isDefined()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, GetPolygon2d)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_EQ(polygon2d, Polygon(polygon2d, origin, xAxis, yAxis).getPolygon2d()) ;

    }

    {

        EXPECT_ANY_THROW(Polygon::Undefined().getOrigin()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, GetOrigin)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_EQ(origin, Polygon(polygon2d, origin, xAxis, yAxis).getOrigin()) ;

    }

    {

        EXPECT_ANY_THROW(Polygon::Undefined().getOrigin()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, GetXAxis)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_EQ(xAxis, Polygon(polygon2d, origin, xAxis, yAxis).getXAxis()) ;

    }

    {

        EXPECT_ANY_THROW(Polygon::Undefined().getXAxis()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, GetYAxis)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_EQ(yAxis, Polygon(polygon2d, origin, xAxis, yAxis).getYAxis()) ;

    }

    {

        EXPECT_ANY_THROW(Polygon::Undefined().getYAxis()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, GetNormalVector)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    
    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_EQ(Vector3d(0.0, 0.0, +1.0), Polygon(polygon2d, origin, xAxis, yAxis).getNormalVector()) ;

    }

    {

        EXPECT_ANY_THROW(Polygon::Undefined().getNormalVector()) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, Translate)
{

    using library::math::obj::Vector3d ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        Polygon polygon = { polygon2d, origin, xAxis, yAxis } ;

        polygon.translate({ 4.0, 5.0, 6.0 }) ;

        EXPECT_EQ(Polygon({ polygon2d, { 5.0, 7.0, 9.0 }, xAxis, yAxis }), polygon) ;

    }

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_ANY_THROW(Polygon::Undefined().translate(Vector3d::Undefined())) ;
        EXPECT_ANY_THROW(Polygon::Undefined().translate({ 0.0, 0.0, 0.0 })) ;
        EXPECT_ANY_THROW(Polygon(polygon2d, origin, xAxis, yAxis).translate(Vector3d::Undefined())) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, Rotate)
{

    using library::core::types::Real ;

    using library::math::obj::Vector3d ;
    using library::math::geom::Angle ;
    using Polygon2d = library::math::geom::d2::objects::Polygon ;
    using library::math::geom::d3::objects::Point ;
    using library::math::geom::d3::objects::Polygon ;
    using library::math::geom::trf::rot::Quaternion ;
    using library::math::geom::trf::rot::RotationVector ;

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        Polygon polygon = { polygon2d, origin, xAxis, yAxis } ;

        polygon.rotate(Quaternion::RotationVector(RotationVector({ 1.0, 0.0, 0.0 }, Angle::Degrees(90.0)))) ;

        const Polygon2d referencePolygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point referenceOrigin = { 1.0, 2.0, 3.0 } ;
        const Vector3d referenceXAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d referenceYAxis = { 0.0, 0.0, -1.0 } ;

        const Polygon referencePolygon = { referencePolygon2d, referenceOrigin, referenceXAxis, referenceYAxis } ;

        EXPECT_TRUE(polygon.getOrigin().isNear(referencePolygon.getOrigin(), Real::Epsilon())) ;
        EXPECT_TRUE(polygon.getXAxis().isApprox(referencePolygon.getXAxis(), Real::Epsilon())) ;
        EXPECT_TRUE(polygon.getYAxis().isApprox(referencePolygon.getYAxis(), Real::Epsilon())) ;
        EXPECT_TRUE(polygon.getNormalVector().isApprox(referencePolygon.getNormalVector(), Real::Epsilon())) ;

    }

    {

        const Polygon2d polygon2d = { { { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 } } } ;
        const Point origin = { 1.0, 2.0, 3.0 } ;
        const Vector3d xAxis = { 1.0, 0.0, 0.0 } ;
        const Vector3d yAxis = { 0.0, 1.0, 0.0 } ;

        EXPECT_ANY_THROW(Polygon::Undefined().rotate(Quaternion::Undefined())) ;
        EXPECT_ANY_THROW(Polygon::Undefined().rotate(Quaternion::Unit())) ;
        EXPECT_ANY_THROW(Polygon(polygon2d, origin, xAxis, yAxis).rotate(Quaternion::Undefined())) ;

    }

}

TEST (Library_Mathematics_Geometry_3D_Objects_Polygon, Undefined)
{

    using library::math::geom::d3::objects::Polygon ;
    
    {

        EXPECT_NO_THROW(Polygon::Undefined()) ;
        EXPECT_FALSE(Polygon::Undefined().isDefined()) ;

    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////