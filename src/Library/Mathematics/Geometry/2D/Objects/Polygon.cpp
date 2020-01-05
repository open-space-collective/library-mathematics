////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Mathematics
/// @file           Library/Mathematics/Geometry/2D/Objects/Polygon.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Library/Mathematics/Geometry/2D/Transformation.hpp>
#include <Library/Mathematics/Geometry/2D/Objects/Polygon.hpp>

#include <Library/Core/Types/String.hpp>
#include <Library/Core/Types/Size.hpp>
#include <Library/Core/Types/Index.hpp>
#include <Library/Core/Error.hpp>
#include <Library/Core/Utilities.hpp>

#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <boost/geometry/strategies/transform.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry.hpp>
#include <boost/numeric/ublas/matrix.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace library
{
namespace math
{
namespace geom
{
namespace d2
{
namespace objects
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using boost::geometry::cs::cartesian ;
using boost::geometry::model::point ;
using boost::geometry::model::ring ;
using boost::geometry::model::polygon ;

using library::core::types::Index ;
using library::core::types::Size ;
using library::core::types::String ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Polygon::Impl
{

    public:

                                Impl                                        (   const   Array<Point>&               anOuterRing,
                                                                                const   Array<Array<Point>>&        anInnerRingArray                            ) ;

                                ~Impl                                       ( ) = default ;

        bool                    operator ==                                 (   const   Polygon::Impl&              aPolygon                                    ) const ;

        bool                    isDefined                                   ( ) const ;

        bool                    intersects                                  (   const   Polygon&                    aPolygon                                    ) const ;

        bool                    contains                                    (   const   Point&                      aPoint                                      ) const ;

        bool                    contains                                    (   const   PointSet&                   aPointSet                                   ) const ;

        Size                    getInnerRingCount                           ( ) const ;

        Array<Polygon::Vertex>  getOuterRingVertices                        ( ) const ;

        Array<Polygon::Vertex>  getInnerRingVerticesAt                      (   const   Index&                      aRingIndex                                  ) const ;

        Size                    getEdgeCount                                ( ) const ;

        Size                    getOuterRingEdgeCount                       ( ) const ;

        Size                    getVertexCount                              ( ) const ;

        Polygon::Ring           getOuterRing                                ( ) const ;

        Polygon::Ring           getInnerRingAt                              (   const   Index&                      anInnerRingIndex                            ) const ;

        Polygon::Edge           getEdgeAt                                   (   const   Index                       anEdgeIndex                                 ) const ;

        Polygon::Vertex         getVertexAt                                 (   const   Index                       aVertexIndex                                ) const ;

        Array<Polygon::Edge>    getEdges                                    ( ) const ;

        Array<Polygon::Vertex>  getVertices                                 ( ) const ;

        // Intersection            intersectionWith                            (   const   Polygon&                    aPolygon                                    ) const ;

        // MultiPolygon            unionWith                                   (   const   Polygon&                    aPolygon                                    ) const ;

        String                  toString                                    (   const   Object::Format&             aFormat,
                                                                                const   Integer&                    aPrecision                                  ) const ;

        void                    applyTransformation                         (   const   Transformation&             aTransformation                             ) ;

    private:

        typedef                 point<double, 2, cartesian>                     BoostPoint ;
        typedef                 ring<Impl::BoostPoint>                          BoostRing ;
        typedef                 polygon<Impl::BoostPoint>                       BoostPolygon ;

        Impl::BoostPolygon      polygon_ ;

        static Impl::BoostPolygon BoostPolygonFromPoints                    (   const   Array<Point>&               aPointArray                                 ) ;

} ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Polygon::Impl::Impl                         (   const   Array<Point>&               anOuterRing,
                                                                                const   Array<Array<Point>>&        anInnerRingArray                            )
                                :   polygon_(Polygon::Impl::BoostPolygonFromPoints(anOuterRing))
{

    for (const auto& innerRing : anInnerRingArray) // [TBM] This is temporary, should be constructed inline instead
    {

        if (innerRing.getSize() < 3)
        {
            throw library::core::error::RuntimeError("At least 3 points are necessary to define an inner ring.") ;
        }

        Polygon::Impl::BoostRing ring ;

        for (const auto& innerRingPoint : innerRing)
        {
            boost::geometry::append(ring, Polygon::Impl::BoostPoint(innerRingPoint.x(), innerRingPoint.y())) ;
        }

        boost::geometry::correct(ring) ;

        polygon_.inners().push_back(ring) ;

    }

}

bool                            Polygon::Impl::operator ==                  (   const   Polygon::Impl&              aPolygon                                    ) const
{
    return boost::geometry::equals(polygon_, aPolygon.polygon_) ;
}

bool                            Polygon::Impl::isDefined                    ( ) const
{
    return polygon_.outer().size() >= 3 ;
}

bool                            Polygon::Impl::intersects                   (   const   Polygon&                    aPolygon                                    ) const
{

    try
    {
        return boost::geometry::intersects(polygon_, aPolygon.implUPtr_->polygon_) ;
    }
    catch (const std::exception& anException)
    {
        throw library::core::error::RuntimeError("Error when checking if polygon intersects polygon: [{}]", anException.what()) ;
    }

    return false ;

}

bool                            Polygon::Impl::contains                     (   const   Point&                      aPoint                                      ) const
{

    try
    {
        return boost::geometry::covered_by(Polygon::Impl::BoostPoint(aPoint.x(), aPoint.y()), polygon_) ;
    }
    catch (const std::exception& anException)
    {
        throw library::core::error::RuntimeError("Error when checking if polygon contains point: [{}]", anException.what()) ;
    }

    return false ;

}

bool                            Polygon::Impl::contains                     (   const   PointSet&                   aPointSet                                   ) const
{

    for (const auto& point : aPointSet)
    {

        if (!this->contains(point))
        {
            return false ;
        }

    }

    return true ;

}

Size                            Polygon::Impl::getInnerRingCount            ( ) const
{
    return polygon_.inners().size() ;
}

Array<Polygon::Vertex>          Polygon::Impl::getOuterRingVertices         ( ) const
{

    if (polygon_.outer().empty())
    {
        return Array<Point>::Empty() ;
    }

    Array<Point> outerRingVertices = Array<Point>::Empty() ;

    for (size_t vertexIdx = 0; vertexIdx < (polygon_.outer().size() - 1); ++vertexIdx)
    {
        outerRingVertices.add(Point(boost::geometry::get<0>(polygon_.outer().at(vertexIdx)), boost::geometry::get<1>(polygon_.outer().at(vertexIdx)))) ;
    }

    return outerRingVertices ;

}

Array<Polygon::Vertex>          Polygon::Impl::getInnerRingVerticesAt       (   const   Index&                      aRingIndex                                  ) const
{

    if (aRingIndex >= this->getInnerRingCount())
    {
        throw library::core::error::RuntimeError("Inner ring index [{}] out of bounds [{}].", aRingIndex, this->getInnerRingCount()) ;
    }

    if (polygon_.inners().at(aRingIndex).empty())
    {
        return Array<Point>::Empty() ;
    }

    Array<Point> innerRingVertices = Array<Point>::Empty() ;

    for (size_t vertexIdx = 0; vertexIdx < (polygon_.inners().at(aRingIndex).size() - 1); ++vertexIdx)
    {
        innerRingVertices.add(Point(boost::geometry::get<0>(polygon_.inners().at(aRingIndex).at(vertexIdx)), boost::geometry::get<1>(polygon_.inners().at(aRingIndex).at(vertexIdx)))) ;
    }

    return innerRingVertices ;

}

Size                            Polygon::Impl::getEdgeCount                 ( ) const
{

    Size edgeCount = boost::geometry::num_points(polygon_.outer()) - 1 ;

    for (const auto& innerRing : polygon_.inners())
    {
        edgeCount += boost::geometry::num_points(innerRing) - 1 ;
    }

    return edgeCount ;

}

Size                            Polygon::Impl::getOuterRingEdgeCount        ( ) const
{
    return boost::geometry::num_points(polygon_.outer()) - 1 ;
}

Size                            Polygon::Impl::getVertexCount               ( ) const
{
    return boost::geometry::num_points(polygon_) - (1 + this->getInnerRingCount()) ;
}

Polygon::Ring                   Polygon::Impl::getOuterRing                 ( ) const
{

    Array<Point> ringVertices = this->getOuterRingVertices() ;

    if (!ringVertices.isEmpty())
    {
        ringVertices.add(ringVertices[0]) ;
    }

    return { ringVertices } ;

}

Polygon::Ring                   Polygon::Impl::getInnerRingAt               (   const   Index&                      anInnerRingIndex                            ) const
{

    Array<Point> ringVertices = this->getInnerRingVerticesAt(anInnerRingIndex) ;

    if (!ringVertices.isEmpty())
    {
        ringVertices.add(ringVertices[0]) ;
    }

    return { ringVertices } ;

}

Polygon::Edge                   Polygon::Impl::getEdgeAt                    (   const   Index                       anEdgeIndex                                 ) const
{

    if (anEdgeIndex >= this->getEdgeCount())
    {
        throw library::core::error::runtime::Wrong("Edge index") ;
    }

    if (anEdgeIndex >= this->getOuterRingEdgeCount())
    {
        throw library::core::error::runtime::ToBeImplemented("Inner ring edge access.") ;
    }

    const Point firstVertex = this->getVertexAt(anEdgeIndex) ;
    const Point secondVertex = (anEdgeIndex != (this->getOuterRingEdgeCount() - 1)) ? this->getVertexAt(anEdgeIndex + 1) : this->getVertexAt(0) ;

    return { firstVertex, secondVertex } ;

}

Polygon::Vertex                 Polygon::Impl::getVertexAt                  (   const   Index                       aVertexIndex                                ) const
{

    if (aVertexIndex >= (boost::geometry::num_points(polygon_) - 1))
    {
        throw library::core::error::runtime::Wrong("Vertex index") ;
    }

    if (aVertexIndex >= (polygon_.outer().size() - 1))
    {
        throw library::core::error::runtime::ToBeImplemented("Inner ring vertex access.") ;
    }

    return { boost::geometry::get<0>(polygon_.outer().at(aVertexIndex)), boost::geometry::get<1>(polygon_.outer().at(aVertexIndex)) } ;

}

Array<Polygon::Edge>            Polygon::Impl::getEdges                     ( ) const
{

    Array<Polygon::Edge> edges = Array<Polygon::Edge>::Empty() ;

    edges.reserve(this->getEdgeCount()) ;

    for (Index edgeIndex = 0; edgeIndex < this->getEdgeCount(); ++edgeIndex)
    {
        edges.add(this->getEdgeAt(edgeIndex)) ;
    }

    return edges ;

}

Array<Polygon::Vertex>          Polygon::Impl::getVertices                  ( ) const
{

    Array<Polygon::Vertex> vertices = Array<Polygon::Vertex>::Empty() ;

    for (const auto& vertex : this->getOuterRingVertices())
    {
        vertices.add(vertex) ;
    }

    for (Index innerRingIndex = 0; innerRingIndex < this->getInnerRingCount(); ++innerRingIndex)
    {
        for (const auto& vertex : this->getInnerRingVerticesAt(innerRingIndex))
        {
            vertices.add(vertex) ;
        }
    }

    return vertices ;

}

String                          Polygon::Impl::toString                     (   const   Object::Format&             aFormat,
                                                                                const   Integer&                    aPrecision                                  ) const
{

    switch (aFormat)
    {

        case Object::Format::Standard:
        case Object::Format::WKT:
        {

            std::stringstream stringStream ;

            if (aPrecision.isDefined())
            {
                stringStream << std::fixed << std::setprecision(aPrecision) << boost::geometry::wkt(polygon_) ;
            }
            else
            {
                stringStream << boost::geometry::wkt(polygon_) ;
            }

            return stringStream.str() ;

        }

        default:
            throw library::core::error::runtime::Wrong("Format") ;
            break ;

    }

    return String::Empty() ;

}

void                            Polygon::Impl::applyTransformation          (   const   Transformation&             aTransformation                             )
{

    using library::math::obj::Matrix3d ;

    Polygon::Impl::BoostPolygon transformedPolygon ;

    const Matrix3d transformationMatrix = aTransformation.getMatrix() ;

    const boost::geometry::strategy::transform::matrix_transformer<double, 2, 2> transform = { transformationMatrix(0, 0), transformationMatrix(0, 1), transformationMatrix(0, 2),
                                                                                               transformationMatrix(1, 0), transformationMatrix(1, 1), transformationMatrix(1, 2),
                                                                                               transformationMatrix(2, 0), transformationMatrix(2, 1), transformationMatrix(2, 2) } ;

    boost::geometry::transform(polygon_, transformedPolygon, transform) ;

    polygon_ = transformedPolygon ;

}

Polygon::Impl::BoostPolygon     Polygon::Impl::BoostPolygonFromPoints       (   const   Array<Point>&               aPointArray                                 )
{

    if ((!aPointArray.isEmpty()) && (aPointArray.getSize() < 3))
    {
        throw library::core::error::RuntimeError("At least 3 points are necessary to define a polygon.") ;
    }

    Polygon::Impl::BoostPolygon polygon ;

    for (const auto& point : aPointArray)
    {
        boost::geometry::append(polygon, Polygon::Impl::BoostPoint(point.x(), point.y())) ;
    }

    boost::geometry::correct(polygon) ;

    return polygon ;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Polygon::Polygon                            (   const   Array<Point>&               anOuterRing,
                                                                                const   Array<Array<Point>>&        anInnerRingArray                            )
                                :   Object(),
                                    implUPtr_(std::make_unique<Polygon::Impl>(anOuterRing, anInnerRingArray))
{

}

                                Polygon::Polygon                            (   const   Polygon&                    aPolygon                                    )
                                :   Object(aPolygon),
                                    implUPtr_((aPolygon.implUPtr_ != nullptr) ? new Polygon::Impl(*aPolygon.implUPtr_) : nullptr)
{

}

                                Polygon::~Polygon                           ( )
{

}

Polygon&                        Polygon::operator =                         (   const   Polygon&                    aPolygon                                    )
{

    if (this != &aPolygon)
    {

        Object::operator = (aPolygon) ;

        implUPtr_.reset((aPolygon.implUPtr_ != nullptr) ? new Polygon::Impl(*aPolygon.implUPtr_) : nullptr) ;

    }

    return *this ;

}

Polygon*                        Polygon::clone                              ( ) const
{
    return new Polygon(*this) ;
}

bool                            Polygon::operator ==                        (   const   Polygon&                    aPolygon                                    ) const
{

    if ((!this->isDefined()) || (!aPolygon.isDefined()))
    {
        return false ;
    }

    return (*implUPtr_) == (*aPolygon.implUPtr_) ;

}

bool                            Polygon::operator !=                        (   const   Polygon&                    aPolygon                                    ) const
{
    return !((*this) == aPolygon) ;
}

bool                            Polygon::isDefined                          ( ) const
{
    return (implUPtr_ != nullptr) && implUPtr_->isDefined() ;
}

bool                            Polygon::intersects                         (   const   Polygon&                    aPolygon                                    ) const
{

    if (!aPolygon.isDefined())
    {
        throw library::core::error::runtime::Undefined("Point") ;
    }

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->intersects(aPolygon) ;

}

bool                            Polygon::contains                           (   const   Point&                      aPoint                                      ) const
{

    if (!aPoint.isDefined())
    {
        throw library::core::error::runtime::Undefined("Point") ;
    }

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->contains(aPoint) ;

}

bool                            Polygon::contains                           (   const   PointSet&                   aPointSet                                   ) const
{

    if (!aPointSet.isDefined())
    {
        throw library::core::error::runtime::Undefined("Point set") ;
    }

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->contains(aPointSet) ;

}

Size                            Polygon::getInnerRingCount                  ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getInnerRingCount() ;

}

Size                            Polygon::getEdgeCount                       ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getEdgeCount() ;

}

Size                            Polygon::getVertexCount                     ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getVertexCount() ;

}

Polygon::Ring                   Polygon::getOuterRing                       ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getOuterRing() ;

}

Polygon::Ring                   Polygon::getInnerRingAt                     (   const   Index&                      anInnerRingIndex                            ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getInnerRingAt(anInnerRingIndex) ;

}

Polygon::Edge                   Polygon::getEdgeAt                          (   const   Index                       anEdgeIndex                                 ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getEdgeAt(anEdgeIndex) ;

}

Polygon::Vertex                 Polygon::getVertexAt                        (   const   Index                       aVertexIndex                                ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getVertexAt(aVertexIndex) ;

}

Array<Polygon::Edge>            Polygon::getEdges                           ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getEdges() ;

}

Array<Polygon::Vertex>          Polygon::getVertices                        ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->getVertices() ;

}

void                            Polygon::print                              (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? library::core::utils::Print::Header(anOutputStream, "Polygon") : void () ;

    library::core::utils::Print::Separator(anOutputStream, "Outer Ring") ;

    if (implUPtr_ != nullptr)
    {

        for (const auto& point : implUPtr_->getOuterRingVertices())
        {
            library::core::utils::Print::Line(anOutputStream) << String::Format("- {}", point.toString()) ;
        }

    }
    else
    {
        library::core::utils::Print::Line(anOutputStream) << "Undefined" ;
    }

    library::core::utils::Print::Separator(anOutputStream, "Inner Rings") ;

    if (implUPtr_ != nullptr)
    {

        for (Index innerRingIndex = 0; innerRingIndex < implUPtr_->getInnerRingCount(); ++innerRingIndex)
        {

            library::core::utils::Print::Separator(anOutputStream, String::Format("Inner Ring @ {}", innerRingIndex)) ;

            for (const auto& point : implUPtr_->getInnerRingVerticesAt(innerRingIndex))
            {
                library::core::utils::Print::Line(anOutputStream) << String::Format("- {}", point.toString()) ;
            }

        }

    }
    else
    {
        library::core::utils::Print::Line(anOutputStream) << "Undefined" ;
    }

    displayDecorators ? library::core::utils::Print::Footer(anOutputStream) : void () ;

}

String                          Polygon::toString                           (   const   Object::Format&             aFormat,
                                                                                const   Integer&                    aPrecision                                  ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    return implUPtr_->toString(aFormat, aPrecision) ;

}

// Intersection                    Polygon::intersectionWith                   (   const   Polygon&                    aPolygon                                    ) const
// {

//     if (!this->isDefined())
//     {
//         throw library::core::error::runtime::Undefined("Polygon") ;
//     }

// }

// MultiPolygon                    Polygon::unionWith                          (   const   Polygon&                    aPolygon                                    ) const
// {

//     if (!this->isDefined())
//     {
//         throw library::core::error::runtime::Undefined("Polygon") ;
//     }

// }

void                            Polygon::applyTransformation                (   const   Transformation&             aTransformation                             )
{

    if (!aTransformation.isDefined())
    {
        throw library::core::error::runtime::Undefined("Transformation") ;
    }

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Polygon") ;
    }

    implUPtr_->applyTransformation(aTransformation) ;

}

Polygon                         Polygon::Undefined                          ( )
{
    return { Array<Point>::Empty() } ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
