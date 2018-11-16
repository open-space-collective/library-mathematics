////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Library/Mathematics
/// @file           Library/Mathematics/Geometry/3D/Intersection.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Library/Mathematics/Geometry/3D/Intersection.hpp>

#include <Library/Core/Error.hpp>
#include <Library/Core/Utilities.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace library
{
namespace math
{
namespace geom
{
namespace d3
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Intersection::Intersection                  (           Array<Unique<Object>>&&     anObjectArray                               )
                                :   type_(Intersection::TypeFromObjects(anObjectArray)),
                                    composite_(std::move(anObjectArray))
{

}

                                Intersection::Intersection                  (   const   Intersection&               anIntersection                              )
                                :   type_(anIntersection.type_),
                                    composite_(anIntersection.composite_)
{

}

                                Intersection::~Intersection                 ( )
{

}

Intersection&                   Intersection::operator =                    (   const   Intersection&               anIntersection                              )
{

    if (this != &anIntersection)
    {

        type_ = anIntersection.type_ ;
        composite_ = anIntersection.composite_ ;

    }

    return *this ;

}

bool                            Intersection::operator ==                   (   const   Intersection&               anIntersection                              ) const
{

    if ((!this->isDefined()) || (!anIntersection.isDefined()))
    {
        return false ;
    }

    return (type_ == anIntersection.type_) && (composite_ == anIntersection.composite_) ;

}

bool                            Intersection::operator !=                   (   const   Intersection&               anIntersection                              ) const
{
    return !((*this) == anIntersection) ;
}

Intersection                    Intersection::operator +                    (   const   Intersection&               anIntersection                              ) const
{

    if (!anIntersection.isDefined())
    {
        throw library::core::error::runtime::Undefined("Intersection") ;
    }

    Intersection intersection ;

    intersection.composite_ = composite_ + anIntersection.composite_ ;

    intersection.type_ = Intersection::TypeFromObjects(intersection.composite_.accessObjects()) ;
    
    return intersection ;

}

Intersection&                   Intersection::operator +=                   (   const   Intersection&               anIntersection                              )
{

    if (!anIntersection.isDefined())
    {
        throw library::core::error::runtime::Undefined("Intersection") ;
    }

    composite_ += anIntersection.composite_ ;

    type_ = Intersection::TypeFromObjects(composite_.accessObjects()) ;
    
    return *this ;

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Intersection&               anIntersection                              )
{

    library::core::utils::Print::Header(anOutputStream, "Intersection") ;

    library::core::utils::Print::Line(anOutputStream) << "Type:"                << Intersection::StringFromType(anIntersection.type_) ;
    
    library::core::utils::Print::Line(anOutputStream) << "Composite:" ;

    anIntersection.composite_.print(anOutputStream, false) ;

    library::core::utils::Print::Footer(anOutputStream) ;

    return anOutputStream ;

}

bool                            Intersection::isDefined                     ( ) const
{
    return (type_ != Intersection::Type::Undefined) && composite_.isDefined() ;
}

bool                            Intersection::isEmpty                       ( ) const
{
    return type_ == Intersection::Type::Empty ;
}

bool                            Intersection::isComplex                     ( ) const
{
    return type_ == Intersection::Type::Complex ;
}

const Composite&                Intersection::accessComposite               ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Intersection") ;
    }
    
    return composite_ ;

}

Intersection::Type              Intersection::getType                       ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Intersection") ;
    }
    
    return type_ ;

}

Intersection                    Intersection::Undefined                     ( )
{
    
    Intersection intersection ;

    intersection.type_ = Intersection::Type::Undefined ;

    return intersection ;

}

Intersection                    Intersection::Empty                         ( )
{
    return { Array<Unique<Object>>::Empty() } ;
}

Intersection                    Intersection::Point                         (   const   objects::Point&             aPoint                                      )
{

    Intersection intersection ;

    intersection.type_ = Intersection::Type::Point ;

    intersection.composite_ = Composite { aPoint } ;

    return intersection ;

}

Intersection                    Intersection::PointSet                      (   const   objects::PointSet&          aPointSet                                   )
{

    Intersection intersection ;

    intersection.type_ = Intersection::Type::PointSet ;
    
    intersection.composite_ = Composite { aPointSet } ;

    return intersection ;

}

Intersection                    Intersection::LineString                    (   const   objects::LineString&        aLineString                                 )
{

    Intersection intersection ;

    intersection.type_ = Intersection::Type::LineString ;
    
    intersection.composite_ = Composite { aLineString } ;

    return intersection ;

}

String                          Intersection::StringFromType                (   const   Intersection::Type&         aType                                       )
{

    switch (aType)
    {

        case Intersection::Type::Undefined:
            return "Undefined" ;

        case Intersection::Type::Empty:
            return "Empty" ;

        case Intersection::Type::Point:
            return "Point" ;

        case Intersection::Type::PointSet:
            return "PointSet" ;

        case Intersection::Type::Line:
            return "Line" ;

        case Intersection::Type::Ray:
            return "Ray" ;

        case Intersection::Type::Segment:
            return "Segment" ;

        case Intersection::Type::LineString:
            return "LineString" ;

        case Intersection::Type::Polygon:
            return "Polygon" ;

        case Intersection::Type::Plane:
            return "Plane" ;

        case Intersection::Type::Sphere:
            return "Sphere" ;

        case Intersection::Type::Ellipsoid:
            return "Ellipsoid" ;

        case Intersection::Type::Pyramid:
            return "Pyramid" ;

        case Intersection::Type::Complex:
            return "Complex" ;

        default:
            throw library::core::error::runtime::Wrong("Type") ;
            break ;

    }

    return String::Empty() ;

}

                                Intersection::Intersection                  ( )
                                :   type_(Intersection::Type::Undefined),
                                    composite_(Composite::Undefined())
{

}

Intersection::Type              Intersection::TypeFromObjects               (   const   Array<Unique<Object>>&      anObjectArray                               )
{

    if (anObjectArray.isEmpty())
    {
        return Intersection::Type::Empty ;
    }

    Intersection::Type type = Intersection::Type::Undefined ;

    for (const auto& objectUPtr : anObjectArray)
    {

        const Intersection::Type objectType = Intersection::TypeFromObject(objectUPtr) ;

        if (type == Intersection::Type::Undefined)
        {
            type = objectType ;
        }
        else if (type != objectType)
        {
            return Intersection::Type::Complex ;
        }

    }

    return type ;

}

Intersection::Type              Intersection::TypeFromObject                (   const   Unique<Object>&             anObjectUPtr                                )
{

    if (dynamic_cast<const objects::Point*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Point ;
    }

    if (dynamic_cast<const objects::PointSet*>(anObjectUPtr.get()))
    {
        return Intersection::Type::PointSet ;
    }

    if (dynamic_cast<const objects::Line*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Line ;
    }

    if (dynamic_cast<const objects::Ray*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Ray ;
    }

    if (dynamic_cast<const objects::Segment*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Segment ;
    }

    if (dynamic_cast<const objects::LineString*>(anObjectUPtr.get()))
    {
        return Intersection::Type::LineString ;
    }

    if (dynamic_cast<const objects::Polygon*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Polygon ;
    }

    if (dynamic_cast<const objects::Plane*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Plane ;
    }

    if (dynamic_cast<const objects::Cuboid*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Cuboid ;
    }

    if (dynamic_cast<const objects::Sphere*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Sphere ;
    }

    if (dynamic_cast<const objects::Ellipsoid*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Ellipsoid ;
    }

    if (dynamic_cast<const objects::Pyramid*>(anObjectUPtr.get()))
    {
        return Intersection::Type::Pyramid ;
    }

    return Intersection::Type::Undefined ;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////