////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Open Space Toolkit ▸ Mathematics
/// @file           OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/PointSet.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Transformation.hpp>
#include <OpenSpaceToolkit/Mathematics/Geometry/3D/Objects/PointSet.hpp>

#include <OpenSpaceToolkit/Core/Error.hpp>
#include <OpenSpaceToolkit/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ostk
{
namespace math
{
namespace geom
{
namespace d3
{
namespace objects
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                PointSet::PointSet                          (   const   Array<Point>&               aPointArray                                 )
                                :   Object(),
                                    points_(aPointArray.begin(), aPointArray.end())
{

}

PointSet*                       PointSet::clone                             ( ) const
{
    return new PointSet(*this) ;
}

bool                            PointSet::operator ==                       (   const   PointSet&                   aPointSet                                   ) const
{
    return points_ == aPointSet.points_ ;
}

bool                            PointSet::operator !=                       (   const   PointSet&                   aPointSet                                   ) const
{
    return !((*this) == aPointSet) ;
}

bool                            PointSet::isDefined                         ( ) const
{
    return !points_.empty() ;
}

bool                            PointSet::isEmpty                           ( ) const
{
    return points_.empty() ;
}

bool                            PointSet::isNear                            (   const   PointSet&                   aPointSet,
                                                                                const   Real&                       aTolerance                                  ) const
{

    if ((this->isEmpty()) || (aPointSet.isEmpty()))
    {
        return false ;
    }

    if (this->getSize() != aPointSet.getSize())
    {
        return false ;
    }

    PointSet::Container firstPointSetBuffer = points_ ;
    PointSet::Container secondPointSetBuffer = aPointSet.points_ ;

    while (!firstPointSetBuffer.empty())
    {

        const Point& firstPoint = *(firstPointSetBuffer.begin()) ;

        bool foundClosestPoint = false ;

        for (auto secondPointSetBufferIt = secondPointSetBuffer.begin(); secondPointSetBufferIt != secondPointSetBuffer.end(); ++secondPointSetBufferIt)
        {

            const Point& secondPoint = *secondPointSetBufferIt ;

            if (firstPoint.isNear(secondPoint, aTolerance))
            {

                foundClosestPoint = true ;

                secondPointSetBuffer.erase(secondPointSetBufferIt) ;

                break ;

            }

        }

        if (foundClosestPoint)
        {
            firstPointSetBuffer.erase(firstPointSetBuffer.begin()) ;
        }
        else
        {
            return false ;
        }

    }

    return true ;

}

Size                            PointSet::getSize                           ( ) const
{
    return points_.size() ;
}

Point                           PointSet::getPointClosestTo                 (   const   Point&                      aPoint                                      ) const
{

    if (!aPoint.isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Point") ;
    }

    if (this->isEmpty())
    {
        throw ostk::core::error::runtime::Undefined("Point set") ;
    }

    Point const* pointPtr = nullptr ;
    Real minDistance = Real::Undefined() ;

    for (const auto& point : points_)
    {

        const Real distance = (point - aPoint).squaredNorm() ;

        if ((!minDistance.isDefined()) || (distance < minDistance))
        {

            pointPtr = &point ;

            minDistance = distance ;

        }

    }

    return *pointPtr ;

}

void                            PointSet::print                             (           std::ostream&               anOutputStream,
                                                                                        bool                        displayDecorators                           ) const
{

    displayDecorators ? ostk::core::utils::Print::Header(anOutputStream, "Point Set") : void () ;

    for (const auto& point : points_)
    {
        ostk::core::utils::Print::Line(anOutputStream)                       << (point.isDefined() ? point.toString() : "Undefined") ;
    }

    displayDecorators ? ostk::core::utils::Print::Footer(anOutputStream) : void () ;

}

PointSet::ConstIterator         PointSet::begin                             ( ) const
{
    return points_.begin() ;
}

PointSet::ConstIterator         PointSet::end                               ( ) const
{
    return points_.end() ;
}

void                            PointSet::applyTransformation               (   const   Transformation&             aTransformation                             )
{

    if (!aTransformation.isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Transformation") ;
    }

    if (!this->isDefined())
    {
        throw ostk::core::error::runtime::Undefined("Point set") ;
    }

    PointSet::Container points ;

    for (auto& point : points_)
    {
        points.insert(aTransformation.applyTo(point)) ;
    }

    points_ = points ;

}

PointSet                        PointSet::Empty                             ( )
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
