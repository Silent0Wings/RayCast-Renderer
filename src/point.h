/**
 * @file point.h
 * @brief Defines the a point class for point operations.
 */
#ifndef POINT_H
#define POINT_H

#include <cmath>

/**
 * @class point
 * @brief Represents a point in 3D space.
 * The point class encapsulates a point defined by x, y, z coordinates.
 * It provides methods to get and set these attributes, as well as to compute the distance
 * to another point, translate the point by a vector, compute the midpoint between two points,
 * and overload operators for addition, subtraction, and multiplication.
 * Inherits from vec3 class.
 */
class point : public vec3
{
public:
    // Default constructor (origin point)
    point(double x = 0, double y = 0, double z = 0) : vec3(x, y, z) {}
    point(std::string str) : vec3(str) {} // String parsing delegated to vec3
    point(const point &other)
    {
        set_x(other.x());
        set_y(other.y());
        set_z(other.z());
    }

    // Getter for x, y, z coordinates (inherited from vec3)
    double get_x() const { return x(); }
    double get_y() const { return y(); }
    double get_z() const { return z(); }

    // Add lexicographical comparison for std::map and other containers
    bool operator<(const point &other) const
    {
        if (x() != other.x())
            return x() < other.x();
        if (y() != other.y())
            return y() < other.y();
        return z() < other.z();
    }

    // Other methods...
    double distance(const vec3 &p1, const vec3 &p2)
    {
        return std::sqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) +
                         (p2.y() - p1.y()) * (p2.y() - p1.y()) +
                         (p2.z() - p1.z()) * (p2.z() - p1.z()));
    }

    void translate(const vec3 &translation_vector) { *this += translation_vector; }

    point midpoint(const point &other) const
    {
        return point((x() + other.x()) / 2.0,
                     (y() + other.y()) / 2.0,
                     (z() + other.z()) / 2.0);
    }

    // Overload operator<< for printing
    friend std::ostream &operator<<(std::ostream &os, const point &p)
    {
        os << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
        return os;
    }

    // Overload operator+ for adding a vector to a point
    friend point operator+(const point &p, const vec3 &v)
    {
        return point(p.x() + v.x(), p.y() + v.y(), p.z() + v.z());
    }

    friend point operator-(const point &p, const vec3 &v)
    {
        return point(p.x() - v.x(), p.y() - v.y(), p.z() - v.z());
    }

    friend vec3 operator-(const point &p1, const point &p2)
    {
        return vec3(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z());
    }

    friend point operator+(const point &p1, const point &p2)
    {
        return point(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
    }

    friend point operator*(const point &p, double t)
    {
        return point(p.x() * t, p.y() * t, p.z() * t);
    }

    friend point operator/(const point &p, double t)
    {
        return point(p.x() / t, p.y() / t, p.z() / t);
    }

    point &operator=(const point &other)
    {
        if (this == &other)
        { // Check for self-assignment
            return *this;
        }

        // Copy data members
        set_x(other.get_x());
        set_y(other.get_y());
        set_z(other.get_z());
        return *this;
    }
};

#endif // POINT_H