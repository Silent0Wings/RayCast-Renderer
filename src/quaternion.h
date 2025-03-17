/**
 * @file quaternion.h
 * @brief Defines the quaternion class for rotation purpouses.
 */

#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"
#include <iostream>

/**
 * @class quaternion
 * @brief Represents a 3D rotation.
 * implements the base for quaternion rotation
 * and its basic operations
 */
class quaternion
{
private:
public:
    double components[4] = {0, 0, 0, 0}; // Declaration

    quaternion() : quaternion(0, 0, 0, 0)
    {
    }
    quaternion(const vec3 &other)
    {
        vec3 temp = gmath::normalize(other);
        set(0, temp.x(), temp.y(), temp.z());
    }
    quaternion(const vec3 *other)
    {
        vec3 temp = gmath::normalize(other);
        set(0, temp.x(), temp.y(), temp.z());
    }

    quaternion(double w_, double x_, double y_, double z_)
    {
        set(w_, x_, y_, z_);
    }
    // Copy constructor based
    quaternion(const quaternion &other) : quaternion(other.w(), other.x(), other.y(), other.z())
    {
    }
    quaternion(const quaternion *other) : quaternion(other->w(), other->x(), other->y(), other->z())
    {
    }

    // getter
    double w() const
    {
        return components[0];
    }
    double x() const
    {
        return components[1];
    }
    double y() const
    {
        return components[2];
    }
    double z() const
    {
        return components[3];
    }

    // setter
    void set_w(double v)
    {
        components[0] = v;
    }
    void set_x(double v)
    {
        components[1] = v;
    }
    void set_y(double v)
    {
        components[2] = v;
    }
    void set_z(double v)
    {
        components[3] = v;
    }

    void set(double w_, double x_, double y_, double z_)
    {
        components[0] = w_;
        components[1] = x_;
        components[2] = y_;
        components[3] = z_;
    }

    void set(vec3 v)
    {
        set(0, v.x(), v.y(), v.z());
    }

    static vec3 rotateVector(const vec3 &v, const quaternion &q)
    {
        quaternion unit_q = q.normalize();
        quaternion v_quat(v);
        quaternion rotated_quat = unit_q * v_quat * unit_q.conjugate();

        return vec3(rotated_quat.x(), rotated_quat.y(), rotated_quat.z());
    }

    static point rotate(point start, double angle, vec3 axis)
    {

        angle = gmath::DegreeToRad(angle); // Convert angle to radians

        // Define a position vector and a rotation axis
        vec3 position = start;

        // Create a quaternion from the axis-angle representation
        quaternion rotationQ = fromAxisAngle(axis, angle);

        // Compute the inverse rotation (quaternion conjugate for unit quaternions)
        quaternion inverseRotationQ = rotationQ.conjugate();

        // Convert position vector to a pure quaternion
        quaternion positionQ(0, position.x(), position.y(), position.z());

        // Apply rotation using quaternion multiplication
        quaternion rotatedQ = rotationQ * positionQ * inverseRotationQ;

        // Extract the rotated position
        vec3 rotatedPosition = vec3(rotatedQ.x(), rotatedQ.y(), rotatedQ.z());

        // Print the results
        // std::cout << "Original Position: " << position << std::endl;
        // std::cout << "Rotated Position: " << rotatedPosition << std::endl;

        return rotatedPosition;
    }

    quaternion
    normalize() const
    {
        double mag = sqrt(magnitudeSquared());
        if (mag == 0)
        {
            throw std::runtime_error("Cannot normalize a zero quaternion");
        }
        return *this / mag;
    }

    static quaternion fromAxisAngle(const vec3 &axis, double angle)
    {
        vec3 used_Axis = gmath::normalize(axis); // Normalizes the axis

        double halfAngle = angle / 2;
        double sinHalf = sin(halfAngle);
        return quaternion(cos(halfAngle),
                          used_Axis.x() * sinHalf,
                          used_Axis.y() * sinHalf,
                          used_Axis.z() * sinHalf);
    }

    quaternion rotateQuaternionByAnother(const quaternion &q1, const quaternion &q2) // repesent the final rotation done by applying 2 rotation to each other
    {
        return q2 * q1 * q2.conjugate(); // rotates q1 by q2
    }

    quaternion rotateQuaternion(const quaternion &q1, const quaternion &q2) // repesent the final rotation done by applying 2 rotation to each other
    {
        return q1 * q2; // this combine the rotation that would have happended if q1 is aplied first followed by q2
    }

    quaternion rotateQuaternion(const vector<quaternion> &vec) // repesent the final rotation done by applying many rotation to each other
    {
        if (vec.size() == 0)
        {
            throw std::invalid_argument("empty vector passed as paramter");
        }
        else if (vec.size() == 1)
        {
            return vec.at(0);
        }
        else
        {
            quaternion temp = vec.at(0);
            for (size_t i = 1; i < vec.size(); i++)
            {
                temp *= vec.at(i); // Applies quaternion multiplication
            }

            return temp;
        }
    }

    quaternion vecToQuaternion(const vec3 &v)
    {
        return quaternion(0, v.x(), v.y(), v.z());
    }

    // Convert quaternion back to vec3
    vec3 quaternionToVec3(const quaternion &q)
    {
        return vec3(q.x(), q.y(), q.z());
    }

    quaternion multiply(const quaternion &q) const
    {
        double nw = w() * q.w() - x() * q.x() - y() * q.y() - z() * q.z();
        double nx = w() * q.x() + x() * q.w() + y() * q.z() - z() * q.y();
        double ny = w() * q.y() - x() * q.z() + y() * q.w() + z() * q.x();
        double nz = w() * q.z() + x() * q.y() - y() * q.x() + z() * q.w();
        return quaternion(nw, nx, ny, nz);
    }

    quaternion multiply(const quaternion &q1, const quaternion &q2) const
    {
        double nw = q2.w() * q1.w() - q2.x() * q1.x() - q2.y() * q1.y() - q2.z() * q1.z();
        double nx = q2.w() * q1.x() + q2.x() * q1.w() + q2.y() * q1.z() - q2.z() * q1.y();
        double ny = q2.w() * q1.y() - q2.x() * q1.z() + q2.y() * q1.w() + q2.z() * q1.x();
        double nz = q2.w() * q1.z() + q2.x() * q1.y() - q2.y() * q1.x() + q2.z() * q1.w();
        return quaternion(nw, nx, ny, nz);
    }

    // Conjugate of the quaternion (used for inversion)
    quaternion conjugate() const
    {
        return quaternion(w(), -x(), -y(), -z());
    }

    // Magnitude squared of the quaternion
    double magnitudeSquared() const
    {
        return w() * w() + x() * x() + y() * y() + z() * z();
    }

    // Inverse of the quaternion (conjugate divided by magnitude squared)
    quaternion inverse() const
    {
        double magSq = magnitudeSquared();
        if (magSq == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }

        return conjugate() * (1.0 / magSq); // Multiply by 1 / magnitude squared
    }

    // Overload operator+
    quaternion operator+(const quaternion &other) const
    {
        return quaternion(components[0] + other.components[0],
                          components[1] + other.components[1],
                          components[2] + other.components[2],
                          components[3] + other.components[3]);
    }

    // Overload operator-
    quaternion operator-(const quaternion &other) const
    {
        return quaternion(components[0] - other.components[0],
                          components[1] - other.components[1],
                          components[2] - other.components[2],
                          components[3] - other.components[3]);
    }

    // Overload operator*
    quaternion operator*(double scalar) const
    {
        return quaternion(components[0] * scalar,
                          components[1] * scalar,
                          components[2] * scalar,
                          components[3] * scalar);
    }

    // Overload operator*
    quaternion operator*(const quaternion &q1) const
    {
        return multiply(q1);
    }

    // Overload operator/

    quaternion operator/(double scalar) const
    {
        if (scalar == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        return quaternion(components[0] / scalar,
                          components[1] / scalar,
                          components[2] / scalar,
                          components[3] / scalar);
    }

    quaternion operator/(const quaternion &q) const
    {
        return *this * q.inverse();
    }

    // overload operator==
    bool operator==(const quaternion &other) const
    {
        return components[0] == other.components[0] &&
               components[1] == other.components[1] &&
               components[2] == other.components[2] &&
               components[3] == other.components[3];
    }

    // overload operator!=
    bool operator!=(const quaternion &other) const
    {
        return !(*this == other);
    }

    // overload operator+=
    quaternion &operator+=(const quaternion &other)
    {
        components[0] += other.components[0];
        components[1] += other.components[1];
        components[2] += other.components[2];
        components[3] += other.components[3];
        return *this;
    }

    // overload operator-=
    quaternion &operator-=(const quaternion &other)
    {
        components[0] -= other.components[0];
        components[1] -= other.components[1];
        components[2] -= other.components[2];
        components[3] -= other.components[3];

        return *this;
    }

    // overload operator*=
    quaternion &operator*=(double scalar)
    {
        components[0] *= scalar;
        components[1] *= scalar;
        components[2] *= scalar;
        components[3] *= scalar;
        return *this;
    }

    quaternion &operator*=(const quaternion &q1)
    {
        *this = multiply(q1);

        return *this;
    }

    // overload operator/=
    quaternion &operator/=(double scalar)
    {
        if (scalar == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        components[0] /= scalar;
        components[1] /= scalar;
        components[2] /= scalar;
        components[3] /= scalar;
        return *this;
    }

    // overload operator=
    quaternion &operator=(const quaternion &other)
    {
        if (this != &other)
        { // Check for self-assignment
            std::copy(std::begin(other.components), std::end(other.components), std::begin(components));
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const quaternion &q)
    {
        os << "(" << q.w() << ", " << q.x() << ", " << q.y() << ", " << q.z() << ")";
        return os;
    }
};
#endif // QUATERNION_H