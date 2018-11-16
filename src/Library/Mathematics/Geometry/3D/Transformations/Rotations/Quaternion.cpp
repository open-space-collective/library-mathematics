////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @project        Library/Mathematics
/// @file           Library/Mathematics/Geometry/3D/Transformations/Rotations/Quaternion.cpp
/// @author         Lucas Brémond <lucas@loftorbital.com>
/// @license        Apache License 2.0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Library/Mathematics/Geometry/3D/Transformations/Rotations/RotationMatrix.hpp>
#include <Library/Mathematics/Geometry/3D/Transformations/Rotations/RotationVector.hpp>
#include <Library/Mathematics/Geometry/3D/Transformations/Rotations/Quaternion.hpp>

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
namespace trf
{
namespace rot
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                Quaternion::Quaternion                      (   const   Real&                       aFirstComponent,
                                                                                const   Real&                       aSecondComponent,
                                                                                const   Real&                       aThirdComponent,
                                                                                const   Real&                       aFourthComponent,
                                                                                const   Quaternion::Format&         aFormat                                     )
                                :   x_((aFormat == Quaternion::Format::XYZS) ? aFirstComponent : aSecondComponent),
                                    y_((aFormat == Quaternion::Format::XYZS) ? aSecondComponent : aThirdComponent),
                                    z_((aFormat == Quaternion::Format::XYZS) ? aThirdComponent : aFourthComponent),
                                    s_((aFormat == Quaternion::Format::XYZS) ? aFourthComponent : aFirstComponent)
{

}

                                Quaternion::Quaternion                      (   const   Vector4d&                   aVector,
                                                                                const   Quaternion::Format&         aFormat                                     )
                                :   x_((aFormat == Quaternion::Format::XYZS) ? aVector(0) : aVector(1)),
                                    y_((aFormat == Quaternion::Format::XYZS) ? aVector(1) : aVector(2)),
                                    z_((aFormat == Quaternion::Format::XYZS) ? aVector(2) : aVector(3)),
                                    s_((aFormat == Quaternion::Format::XYZS) ? aVector(3) : aVector(0))
{

}

                                Quaternion::Quaternion                      (   const   Vector3d&                   aVectorPart,
                                                                                const   Real&                       aScalarPart                                 )
                                :   x_(aVectorPart.x()),
                                    y_(aVectorPart.y()),
                                    z_(aVectorPart.z()),
                                    s_(aScalarPart)
{

}

bool                            Quaternion::operator ==                     (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        return false ;
    }

    return ((x_ == +aQuaternion.x_) && (y_ == +aQuaternion.y_) && (z_ == +aQuaternion.z_) && (s_ == +aQuaternion.s_))
        || ((x_ == -aQuaternion.x_) && (y_ == -aQuaternion.y_) && (z_ == -aQuaternion.z_) && (s_ == -aQuaternion.s_)) ;

}

bool                            Quaternion::operator !=                     (   const   Quaternion&                 aQuaternion                                 ) const
{
    return !((*this) == aQuaternion) ;
}

Quaternion                      Quaternion::operator *                      (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return this->crossMultiply(aQuaternion) ;
    
}

Vector3d                        Quaternion::operator *                      (   const   Vector3d&                   aVector                                     ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    if (!aVector.isDefined())
    {
        throw library::core::error::runtime::Undefined("Vector") ;
    }

    return this->rotateVector(aVector) ;
    
}

Quaternion                      Quaternion::operator /                      (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    if (aQuaternion.norm() < Real::Epsilon())
    {
        throw library::core::error::RuntimeError("Cannot divide by quaternion with zero norm.") ;
    }

    return this->crossMultiply(aQuaternion.toInverse()) ;

}

Quaternion&                     Quaternion::operator *=                     (   const   Quaternion&                 aQuaternion                                 )
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    (*this) = this->crossMultiply(aQuaternion) ; // [TBI] This is a shortcut... could be optimized

    return *this ;

}

Quaternion&                     Quaternion::operator /=                     (   const   Quaternion&                 aQuaternion                                 )
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    if (aQuaternion.norm() < Real::Epsilon())
    {
        throw library::core::error::RuntimeError("Cannot divide by quaternion with zero norm.") ;
    }

    (*this) = this->crossMultiply(aQuaternion.toInverse()) ; // [TBI] This is a shortcut... could be optimized

    return *this ;

}

std::ostream&                   operator <<                                 (           std::ostream&               anOutputStream,
                                                                                const   Quaternion&                 aQuaternion                                 )
{

    library::core::utils::Print::Header(anOutputStream, "Quaternion") ;

    library::core::utils::Print::Line(anOutputStream) << "X:"                   << (aQuaternion.isDefined() ? String::Format("{:15f}", aQuaternion.x_) : "Undefined") ;
    library::core::utils::Print::Line(anOutputStream) << "Y:"                   << (aQuaternion.isDefined() ? String::Format("{:15f}", aQuaternion.y_) : "Undefined") ;
    library::core::utils::Print::Line(anOutputStream) << "Z:"                   << (aQuaternion.isDefined() ? String::Format("{:15f}", aQuaternion.z_) : "Undefined") ;
    library::core::utils::Print::Line(anOutputStream) << "S:"                   << (aQuaternion.isDefined() ? String::Format("{:15f}", aQuaternion.s_) : "Undefined") ;

    library::core::utils::Print::Footer(anOutputStream) ;

    return anOutputStream ;

}

bool                            Quaternion::isDefined                       ( ) const
{
    return x_.isDefined() && y_.isDefined() && z_.isDefined() && s_.isDefined() ;
}

bool                            Quaternion::isUnitary                       ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    return std::abs(((x_ * x_) + (y_ * y_) + (z_ * z_) + (s_ * s_)) - 1.0) <= Real::Epsilon() ;

}

bool                            Quaternion::isNear                          (   const   Quaternion&                 aQuaternion,
                                                                                const   Angle&                      anAngularTolerance                          ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    if (!anAngularTolerance.isDefined())
    {
        throw library::core::error::runtime::Undefined("Angular tolerance") ;
    }

    return this->angularDifferenceWith(aQuaternion).inRadians(0.0, Real::TwoPi()) <= anAngularTolerance.inRadians(0.0, Real::TwoPi()) ;

}

Real                            Quaternion::x                               ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    return x_ ;

}

Real                            Quaternion::y                               ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    return y_ ;

}

Real                            Quaternion::z                               ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    return z_ ;

}

Real                            Quaternion::s                               ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }
    
    return s_ ;

}

Vector3d                        Quaternion::getVectorPart                   ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return { x_, y_, z_ } ;

}

Real                            Quaternion::getScalarPart                   ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return s_ ;

}

Quaternion                      Quaternion::toNormalized                    ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return Quaternion(*this).normalize() ;

}

Quaternion                      Quaternion::toConjugate                     ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return Quaternion(*this).conjugate() ;

}

Quaternion                      Quaternion::toInverse                       ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return Quaternion(*this).inverse() ;

}

// Quaternion                      Quaternion::pow                             (   const   Real&                       aValue                                      ) const
// {

//     if (!this->isDefined())
//     {
//         throw library::core::error::runtime::Undefined("Quaternion") ;
//     }

//     AAAAAAA

// }

// Quaternion                      Quaternion::exp                             ( ) const
// {

//     if (!this->isDefined())
//     {
//         throw library::core::error::runtime::Undefined("Quaternion") ;
//     }

//     AAAAAAA

// }

// Quaternion                      Quaternion::log                             ( ) const
// {

//     if (!this->isDefined())
//     {
//         throw library::core::error::runtime::Undefined("Quaternion") ;
//     }

//     AAAAAAA

// }

Real                            Quaternion::norm                            ( ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return ((x_ * x_) + (y_ * y_) + (z_ * z_) + (s_ * s_)).sqrt() ;

}

Quaternion                      Quaternion::crossMultiply                   (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    const Vector3d leftVectorPart = this->getVectorPart() ;
    const Real& leftScalarPart = s_ ;

    const Vector3d rightVectorPart = aQuaternion.getVectorPart() ;
    const Real& rightScalarPart = aQuaternion.s_ ;

    const Vector3d vectorPart = (rightScalarPart * leftVectorPart) + (leftScalarPart * rightVectorPart) - leftVectorPart.cross(rightVectorPart) ;
    const Real scalarPart = (leftScalarPart * rightScalarPart) - leftVectorPart.dot(rightVectorPart) ;

    return { vectorPart, scalarPart } ;

}

Quaternion                      Quaternion::dotMultiply                     (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    const Vector3d leftVectorPart = this->getVectorPart() ;
    const Real& leftScalarPart = s_ ;

    const Vector3d rightVectorPart = aQuaternion.getVectorPart() ;
    const Real& rightScalarPart = aQuaternion.s_ ;

    const Vector3d vectorPart = (rightScalarPart * leftVectorPart) + (leftScalarPart * rightVectorPart) + leftVectorPart.cross(rightVectorPart) ;
    const Real scalarPart = (leftScalarPart * rightScalarPart) - leftVectorPart.dot(rightVectorPart) ;

    return { vectorPart, scalarPart } ;

}

Vector3d                        Quaternion::rotateVector                    (   const   Vector3d&                   aVector                                     ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    if (!aVector.isDefined())
    {
        throw library::core::error::runtime::Undefined("Vector") ;
    }

    if (!this->isUnitary())
    {
        throw library::core::error::RuntimeError("Quaternion with norm [{}] is not unitary.", this->norm()) ;
    }

    return this->crossMultiply(Quaternion(aVector, 0.0)).crossMultiply(this->toConjugate()).getVectorPart() ;

}

Vector4d                        Quaternion::toVector                        (   const   Quaternion::Format&         aFormat                                     ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    switch (aFormat)
    {

        case Quaternion::Format::XYZS:
            return Vector4d(x_, y_, z_, s_) ;
        
        case Quaternion::Format::SXYZ:
            return Vector4d(s_, x_, y_, z_) ;

        default:
            throw library::core::error::runtime::Wrong("Format") ;
            break ;

    }

    return Vector4d::Undefined() ;

}

String                          Quaternion::toString                        (   const   Quaternion::Format&         aFormat                                     ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return this->toVector(aFormat).toString() ;

}

String                          Quaternion::toString                        (   const   Integer&                    aPrecision,
                                                                                const   Quaternion::Format&         aFormat                                     ) const
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    return aPrecision.isDefined() ? this->toVector(aFormat).toString(aPrecision) : this->toVector(aFormat).toString() ;

}

Quaternion&                     Quaternion::normalize                       ( )
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    const Real norm = this->norm() ;

    if (norm.abs() < Real::Epsilon())
    {
        throw library::core::error::RuntimeError("Quaternion norm is zero.") ;
    }

    x_ /= norm ;
    y_ /= norm ;
    z_ /= norm ;
    s_ /= norm ;

    return *this ;

}

Quaternion&                     Quaternion::conjugate                       ( )
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    x_ = -x_ ;
    y_ = -y_ ;
    z_ = -z_ ;

    return *this ;

}

Quaternion&                     Quaternion::inverse                         ( )
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    const Real reducedNorm = (x_ * x_) + (y_ * y_) + (z_ * z_) + (s_ * s_) ;

    if (reducedNorm.abs() < Real::Epsilon())
    {
        throw library::core::error::RuntimeError("Quaternion norm is zero.") ;
    }

    x_ = -x_ / reducedNorm ;
    y_ = -y_ / reducedNorm ;
    z_ = -z_ / reducedNorm ;
    s_ = +s_ / reducedNorm ;

    return *this ;

}

Quaternion&                     Quaternion::rectify                         ( )
{

    if (!this->isDefined())
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    if (s_ < 0.0)
    {

        x_ = -x_ ;
        y_ = -y_ ;
        z_ = -z_ ;
        s_ = -s_ ;

    }

    return *this ;

}

Angle                           Quaternion::angularDifferenceWith           (   const   Quaternion&                 aQuaternion                                 ) const
{

    if ((!this->isDefined()) || (!aQuaternion.isDefined()))
    {
        throw library::core::error::runtime::Undefined("Quaternion") ;
    }

    if ((!this->isUnitary()) || (!aQuaternion.isUnitary()))
    {
        throw library::core::error::RuntimeError("Quaternion is not unitary.") ;
    }

    const Quaternion deltaQuaternion = ((*this) / aQuaternion).normalize() ;

    return Angle::Radians(2.0 * std::acos(std::abs(deltaQuaternion.s_))) ;

}

Quaternion                      Quaternion::Undefined                       ( )
{
    return { Real::Undefined(), Real::Undefined(), Real::Undefined(), Real::Undefined(), Quaternion::Format::XYZS } ;
}

Quaternion                      Quaternion::Unit                            ( )
{
    return { 0.0, 0.0, 0.0, 1.0, Quaternion::Format::XYZS } ;
}

Quaternion                      Quaternion::XYZS                            (   const   Real&                       aFirstComponent,
                                                                                const   Real&                       aSecondComponent,
                                                                                const   Real&                       aThirdComponent,
                                                                                const   Real&                       aFourthComponent                            )
{
    return { aFirstComponent, aSecondComponent, aThirdComponent, aFourthComponent, Quaternion::Format::XYZS } ;
}

Quaternion                      Quaternion::RotationVector                  (   const   rot::RotationVector&        aRotationVector                             )
{

    /// @ref Markley F. L.: Fundamentals of Spacecraft Attitude Determination and Control, 45

    if (!aRotationVector.isDefined())
    {
        throw library::core::error::runtime::Undefined("Rotation Vector") ;
    }

    const Real rotationAngle_rad = aRotationVector.getAngle().inRadians() ;

    const Vector3d vectorPart = std::sin(rotationAngle_rad / 2.0) * aRotationVector.getAxis() ;
    const Real scalarPart = std::cos(rotationAngle_rad / 2.0) ;

    return Quaternion(vectorPart, scalarPart).normalize() ;

}

Quaternion                      Quaternion::RotationMatrix                  (   const   rot::RotationMatrix&        aRotationMatrix                             )
{

    /// @ref Markley F. L.: Fundamentals of Spacecraft Attitude Determination and Control, 48
    /// @note Should we use this method instead? https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf

    if (!aRotationMatrix.isDefined())
    {
        throw library::core::error::runtime::Undefined("Rotation matrix") ;
    }

    const Real trace = aRotationMatrix.accessMatrix().trace() ;
    
    const Real rotationMatrix_11 = aRotationMatrix(0, 0) ;
    const Real rotationMatrix_12 = aRotationMatrix(0, 1) ;
    const Real rotationMatrix_13 = aRotationMatrix(0, 2) ;
    
    const Real rotationMatrix_21 = aRotationMatrix(1, 0) ;
    const Real rotationMatrix_22 = aRotationMatrix(1, 1) ;
    const Real rotationMatrix_23 = aRotationMatrix(1, 2) ;

    const Real rotationMatrix_31 = aRotationMatrix(2, 0) ;
    const Real rotationMatrix_32 = aRotationMatrix(2, 1) ;
    const Real rotationMatrix_33 = aRotationMatrix(2, 2) ;

    Real x = Real::Undefined() ;
    Real y = Real::Undefined() ;
    Real z = Real::Undefined() ;
    Real s = Real::Undefined() ;

    if ((trace >= rotationMatrix_11) && (trace >= rotationMatrix_22) && (trace >= rotationMatrix_33))
    {

        x = rotationMatrix_23 - rotationMatrix_32 ;
        y = rotationMatrix_31 - rotationMatrix_13 ;
        z = rotationMatrix_12 - rotationMatrix_21 ;
        s = 1.0 + trace ;

    }
    else if ((rotationMatrix_11 >= trace) && (rotationMatrix_11 >= rotationMatrix_22) && (rotationMatrix_11 >= rotationMatrix_33))
    {

        x = 1.0 + 2.0 * rotationMatrix_11 - trace ;
        y = rotationMatrix_12 + rotationMatrix_21 ;
        z = rotationMatrix_13 + rotationMatrix_31 ;
        s = rotationMatrix_23 - rotationMatrix_32 ;

    }
    else if ((rotationMatrix_22 >= rotationMatrix_11) && (rotationMatrix_22 >= trace) && (rotationMatrix_22 >= rotationMatrix_33))
    {

        x = rotationMatrix_21 + rotationMatrix_12 ;
        y = 1.0 + 2.0 * rotationMatrix_22 - trace ;
        z = rotationMatrix_23 + rotationMatrix_32 ;
        s = rotationMatrix_31 - rotationMatrix_13 ;

    }
    else if ((rotationMatrix_33 >= rotationMatrix_11) && (rotationMatrix_33 >= rotationMatrix_22) && (rotationMatrix_33 >= trace))
    {

        x = rotationMatrix_31 + rotationMatrix_13 ;
        y = rotationMatrix_32 + rotationMatrix_23 ;
        z = 1.0 + 2.0 * rotationMatrix_33 - trace ;
        s = rotationMatrix_12 - rotationMatrix_21 ;

    }
    else
    {
        throw library::core::error::RuntimeError("Rotation matrix cannot be converted into quaternion.") ;
    }

    return Quaternion::XYZS(x, y, z, s).normalize() ;

}

Quaternion                      Quaternion::Parse                           (   const   String&                     aString,
                                                                                const   Quaternion::Format&         aFormat                                     )
{

    using library::math::obj::VectorXd ;

    if (aString.isEmpty())
    {
        throw library::core::error::runtime::Undefined("String") ;
    }

    VectorXd vector = VectorXd::Parse(aString) ;

    if (vector.size() != 4)
    {
        throw library::core::error::RuntimeError("Vector size is not 4.") ;
    }

    return { vector, aFormat } ;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
}
}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////