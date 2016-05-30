/*
 * Copyright (C) 2015 Fondazione Istituto Italiano di Tecnologia
 * Authors: Silvio Traversaro
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#include <iDynTree/Core/AngularMotionVector3.h>
#include <iDynTree/Core/ClassicalAcc.h>
#include <iDynTree/Core/Rotation.h>
#include <iDynTree/Core/Axis.h>
#include <iDynTree/Core/Direction.h>
#include <iDynTree/Core/Position.h>
#include <iDynTree/Core/Wrench.h>
#include <iDynTree/Core/RotationalInertiaRaw.h>
#include <iDynTree/Core/Twist.h>
#include <iDynTree/Core/SpatialAcc.h>
#include <iDynTree/Core/SpatialMomentum.h>
#include <iDynTree/Core/Utils.h>
#include <iDynTree/Core/PrivateSemanticsMacros.h>
#include <iDynTree/Core/EigenHelpers.h>


#include <Eigen/Dense>

#include <cassert>
#include <iostream>
#include <sstream>

namespace iDynTree
{
    /**
     * Local static functions
     */

    template <class SpatialMotionForceVectorT>
    static SpatialMotionForceVectorT changeCoordFrameOfT(const Rotation & rot,
                                                         const SpatialMotionForceVectorT & other)
    {
        return SpatialMotionForceVectorT(other.getLinearVec3().changeCoordFrame(rot),
                                         other.getAngularVec3().changeCoordFrame(rot));
    }

    /**
     * class Method definitions
     */


    typedef Eigen::Matrix<double,3,3,Eigen::RowMajor> Matrix3dRowMajor;


    Rotation::Rotation(): RotationRaw()
    {
    }

    Rotation::Rotation(double xx, double xy, double xz,
                       double yx, double yy, double yz,
                       double zx, double zy, double zz): RotationRaw(xx,xy,xz,
                                                                     yx,yy,yz,
                                                                     zx,zy,zz)
    {
    }

    Rotation::Rotation(const Rotation & other): RotationRaw(other)
    {
        iDynTreeSemanticsOp(this->semantics = other.getSemantics());
    }

    Rotation::Rotation(const RotationRaw& other): RotationRaw(other)
    {

    }

    Rotation::Rotation(const RotationRaw& otherPos, RotationSemantics & otherSem): RotationRaw(otherPos)
    {
        iDynTreeSemanticsOp(this->semantics = otherSem);
    }

    RotationSemantics& Rotation::getSemantics()
    {
        return this->semantics;
    }

    const RotationSemantics& Rotation::getSemantics() const
    {
        return this->semantics;
    }

    const Rotation& Rotation::changeOrientFrame(const Rotation& newOrientFrame)
    {
        iDynTreeAssert( this->semantics.changeOrientFrame(newOrientFrame.semantics) );
        this->RotationRaw::changeOrientFrame(newOrientFrame);
        return *this;
    }

    const Rotation& Rotation::changeRefOrientFrame(const Rotation& newRefOrientFrame)
    {
        iDynTreeAssert( this->semantics.changeRefOrientFrame(newRefOrientFrame.semantics) );
        this->RotationRaw::changeRefOrientFrame(newRefOrientFrame);
        return *this;
    }

    const Rotation& Rotation::changeCoordinateFrame(const Rotation& newCoordinateFrame)
    {
        return this->changeRefOrientFrame(newCoordinateFrame);
    }

    Rotation Rotation::compose(const Rotation& op1, const Rotation& op2)
    {
        RotationSemantics resultSemantics;
        iDynTreeAssert( RotationSemantics::compose(op1.semantics,op2.semantics,resultSemantics) );
        return Rotation(RotationRaw::compose(op1,op2),resultSemantics);
    }

    Rotation Rotation::inverse2(const Rotation& orient)
    {
        RotationSemantics resultSemantics;
        iDynTreeAssert( RotationSemantics::inverse2(orient.getSemantics(),resultSemantics) );
        return Rotation(RotationRaw::inverse2(orient),resultSemantics);
    }

    Position Rotation::changeCoordFrameOf(const Position & other) const
    {
        PositionSemantics resultSemantics;
        iDynTreeAssert( this->semantics.changeCoordFrameOf(other.getSemantics(), resultSemantics) );
        return Position(this->RotationRaw::changeCoordFrameOf(other), resultSemantics);
    }

    SpatialMotionVector Rotation::changeCoordFrameOf(const SpatialMotionVector& other) const
    {
        return changeCoordFrameOfT<SpatialMotionVector>(*this, other);
    }

    SpatialForceVector Rotation::changeCoordFrameOf(const SpatialForceVector& other) const
    {
        return changeCoordFrameOfT<SpatialForceVector>(*this, other);
    }

    Twist Rotation::changeCoordFrameOf(const Twist& other) const
    {
        return changeCoordFrameOfT<Twist>(*this, other);
    }

    SpatialAcc Rotation::changeCoordFrameOf(const SpatialAcc & other) const
    {
        return changeCoordFrameOfT<SpatialAcc>(*this, other);
    }

    SpatialMomentum Rotation::changeCoordFrameOf(const SpatialMomentum & other) const
    {
        return changeCoordFrameOfT<SpatialMomentum>(*this, other);
    }

    Wrench Rotation::changeCoordFrameOf(const Wrench &other) const
    {
        return changeCoordFrameOfT<Wrench>(*this, other);
    }

    Direction Rotation::changeCoordFrameOf(const Direction& other) const
    {
        Direction result;

        // \todo TODO add semantics to Direction
        Eigen::Map<const Matrix3dRowMajor> newCoordFrame(m_data);
        Eigen::Map<const Eigen::Vector3d> directionCoord(other.data());
        Eigen::Map<Eigen::Vector3d> resultData(result.data());

        resultData = newCoordFrame*directionCoord;

        return result;
    }

    ClassicalAcc Rotation::changeCoordFrameOf(const ClassicalAcc &other) const
    {
        ClassicalAcc result;

        // \todo TODO add semantics to ClassicalAcc
        result = RotationRaw::changeCoordFrameOf(other);

        return result;
    }

    RotationalInertiaRaw Rotation::changeCoordFrameOf(const RotationalInertiaRaw &other) const
    {
        RotationalInertiaRaw result;

        // \todo TODO add semantics to ClassicalAcc
        result = RotationRaw::changeCoordFrameOf(other);

        return result;
    }

    Axis Rotation::changeCoordFrameOf(const Axis& other) const
    {
        return Axis(this->changeCoordFrameOf(other.getDirection()),this->changeCoordFrameOf(other.getOrigin()));
    }

    Rotation Rotation::inverse() const
    {
        return inverse2(*this);
    }

    Rotation Rotation::operator*(const Rotation& other) const
    {
        return compose(*this,other);
    }

    Position Rotation::operator*(const Position& other) const
    {
        return changeCoordFrameOf(other);
    }

    SpatialForceVector Rotation::operator*(const SpatialForceVector& other) const
    {
        return changeCoordFrameOfT<SpatialForceVector>(*this, other);
    }

    Twist Rotation::operator*(const Twist& other) const
    {
        return changeCoordFrameOfT<Twist>(*this, other);
    }

    SpatialAcc Rotation::operator*(const SpatialAcc & other) const
    {
        return changeCoordFrameOfT<SpatialAcc>(*this, other);
    }

    SpatialMomentum Rotation::operator*(const SpatialMomentum & other) const
    {
        return changeCoordFrameOfT<SpatialMomentum>(*this, other);
    }

    Wrench Rotation::operator*(const Wrench& other) const
    {
        return changeCoordFrameOfT<Wrench>(*this, other);
    }

    Direction Rotation::operator*(const Direction& other) const
    {
        return changeCoordFrameOf(other);
    }

    ClassicalAcc Rotation::operator*(const ClassicalAcc& other) const
    {
        return changeCoordFrameOf(other);
    }

    Axis Rotation::operator*(const Axis& other) const
    {
        return changeCoordFrameOf(other);
    }

    RotationalInertiaRaw Rotation::operator*(const RotationalInertiaRaw& other) const
    {
        return changeCoordFrameOf(other);
    }

    void Rotation::getRPY(double& r, double& p, double& y)
    {
        Eigen::Map<const Matrix3dRowMajor> R(m_data);


        if (R(2,0)<1.0)
        {
            if (R(2,0)>-1.0)
            {
                r=atan2(R(2,1),R(2,2));
                p=asin(-R(2,0));
                y=atan2(R(1,0),R(0,0));
            }
            else
            {
                // Not a unique solution
                r=0.0;
                p=M_PI/2.0;
                y=-atan2(-R(1,2),R(1,1));
            }
        }
        else
        {
            // Not a unique solution
            r=0.0;
            p=-M_PI/2.0;
            y=atan2(-R(1,2),R(1,1));
        }
    }

    bool Rotation::getQuaternion(iDynTree::Vector4& quaternion) const
    {
        return getQuaternion(quaternion(0), quaternion(1), quaternion(2), quaternion(3));
    }

    bool Rotation::getQuaternion(double &s, double &r1, double &r2, double &r3) const
    {
        Eigen::Map<const Matrix3dRowMajor> R(m_data);

        //Taken from "Contributions au contrôle automatique de véhicules aériens"
        //PhD thesis of "Minh Duc HUA"
        //INRIA Sophia Antipolis
        //Equation 3.9 (page 101)

        double q0 = ( R(1,1) + R(2,2) + R(3,3) + 1.0) / 4.0;
        double q1 = ( R(1,1) - R(2,2) - R(3,3) + 1.0) / 4.0;
        double q2 = (-R(1,1) + R(2,2) - R(3,3) + 1.0) / 4.0;
        double q3 = (-R(1,1) - R(2,2) + R(3,3) + 1.0) / 4.0;

        if (q0 < 0.0) q0 = 0.0;
        if (q1 < 0.0) q1 = 0.0;
        if (q2 < 0.0) q2 = 0.0;
        if (q3 < 0.0) q3 = 0.0;

        q0 = std::sqrt(q0);
        q1 = std::sqrt(q1);
        q2 = std::sqrt(q2);
        q3 = std::sqrt(q3);

        if (q0 >= q1 && q0 >= q2 && q0 >= q3) {
            q0 *= 1.0;
            q1 *= (R(3,2) - R(2,3)) >= 0 ? 1 : -1;
            q2 *= (R(1,3) - R(1,1)) >= 0 ? 1 : -1;
            q3 *= (R(2,1) - R(1,2)) >= 0 ? 1 : -1;
        } else if (q1 >= q0 && q1 >= q2 && q1 >= q3) {
            q0 *= (R(3,2) - R(2,3)) >= 0 ? 1 : -1;
            q1 *= 1.0;
            q2 *= (R(2,1) + R(1,2)) >= 0 ? 1 : -1;
            q3 *= (R(1,3) + R(1,1)) >= 0 ? 1 : -1;
        } else if (q2 >= q0 && q2 >= q1 && q2 >= q3) {
            q0 *= (R(1,3) - R(1,1)) >= 0 ? 1 : -1;
            q1 *= (R(2,1) + R(1,2)) >= 0 ? 1 : -1;
            q2 *= 1.0;
            q3 *= (R(3,2) + R(2,3)) >= 0 ? 1 : -1;
        } else if (q3 >= q0 && q3 >= q1 && q3 >= q2) {
            q0 *= (R(2,1) - R(1,2)) >= 0 ? 1 : -1;
            q1 *= (R(1,1) + R(1,3)) >= 0 ? 1 : -1;
            q2 *= (R(3,2) + R(2,3)) >= 0 ? 1 : -1;
            q3 *= 1.0;
        } else {
            reportError("Rotation", "getQuaternion", "Quaternion numerically bad conditioned");
            return false;
        }

        double quaternionNorm = std::sqrt(q0 * q0 +
                                          q1 * q1 +
                                          q2 * q2 +
                                          q3 * q3);
        s  = q0 / quaternionNorm;
        r1 = q1 / quaternionNorm;
        r2 = q2 / quaternionNorm;
        r3 = q3 / quaternionNorm;
        return true;
    }


    AngularMotionVector3 Rotation::log() const
    {
        AngularMotionVector3 ret;

        Eigen::AngleAxisd aa(Eigen::Map<const Matrix3dRowMajor>(this->data()));

        // Implementation inspired from DART, see
        // https://github.com/dartsim/dart/pull/407/files
        // https://github.com/dartsim/dart/pull/334
        // https://github.com/dartsim/dart/issues/88
        Eigen::Map<Eigen::Vector3d>(ret.data()) = aa.angle()*aa.axis();

        return ret;
    }

    Rotation Rotation::RotX(const double angle)
    {
        return Rotation(RotationRaw::RotX(angle));
    }

    Rotation Rotation::RotY(const double angle)
    {
        return Rotation(RotationRaw::RotY(angle));
    }

    Rotation Rotation::RotZ(const double angle)
    {
        return Rotation(RotationRaw::RotZ(angle));
    }

    Rotation Rotation::RotAxis(const Direction & direction, const double angle)
    {
        Rotation result;
        Eigen::Map<Matrix3dRowMajor> thisData(result.data());
        Eigen::Map<const Eigen::Vector3d>   directionData(direction.data());
        thisData = Eigen::AngleAxisd(angle, directionData).matrix();

        return result;
    }

    Matrix3x3 Rotation::RotAxisDerivative(const Direction& direction, const double angle)
    {
        Matrix3x3 result;

        Eigen::Map<Matrix3dRowMajor> res(result.data());
        Eigen::Map<const Eigen::Vector3d> d(direction.data());
        Matrix3dRowMajor skewd = skew(d);

        res = skewd*cos(angle)+skewd*skewd*sin(angle);

        return result;
    }

    Rotation Rotation::RPY(const double roll, const double pitch, const double yaw)
    {
        return Rotation(RotationRaw::RPY(roll, pitch, yaw));
    }

    Rotation Rotation::Identity()
    {
        return RotationRaw::Identity();
    }

    Rotation Rotation::RotationFromQuaternion(const iDynTree::Vector4& quaternion)
    {
        //Taken from "Contributions au contrôle automatique de véhicules aériens"
        //PhD thesis of "Minh Duc HUA"
        //INRIA Sophia Antipolis
        //Equation 3.8 (page 101)

        // Rodriques's formula
        // R = I3 + 2s S(r) + 2S(r)^2,
        Rotation _rotation;
        Eigen::Map<Eigen::Matrix3d> rotation(_rotation.data());
        rotation.setIdentity();

        Matrix3x3 _skewSymmetricMatrix;
        Eigen::Map<Eigen::Matrix3d> skewSymmetricMatrix(_skewSymmetricMatrix.data());
        skewSymmetricMatrix.setZero();
        skewSymmetricMatrix(0, 1) = -quaternion(3);
        skewSymmetricMatrix(0, 2) =  quaternion(2);
        skewSymmetricMatrix(1, 2) = -quaternion(1);

        skewSymmetricMatrix(1, 0) = -skewSymmetricMatrix(0, 1);
        skewSymmetricMatrix(2, 0) = -skewSymmetricMatrix(0, 2);
        skewSymmetricMatrix(2, 1) = -skewSymmetricMatrix(1, 2);

        rotation += 2 * quaternion(0) * skewSymmetricMatrix + 2 * skewSymmetricMatrix * skewSymmetricMatrix;

        return _rotation;
    }

    std::string Rotation::toString() const
    {
        std::stringstream ss;

        ss << RotationRaw::toString();
        iDynTreeSemanticsOp(ss << " " << semantics.toString());

        return ss.str();
    }

    std::string Rotation::reservedToString() const
    {
        return this->toString();
    }


}
