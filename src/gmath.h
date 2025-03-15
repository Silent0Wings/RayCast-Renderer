/**
 * @file gmath.h
 * @brief Defines basic mathematical operations for vectors and points.
 */
#ifndef GMATH_H
#define GMATH_H

#include <cmath>
#include <vector>
#include "vec3.h"
#include "point.h"
#include "ray.h"
#include <math.h>
#include "Hit.h"

/**
 * @class gmath
 * @brief Represents a collection of mathematical operations for vectors and points.
 * The gmath class encapsulates a collection of mathematical operations for vectors and points.
 * It provides methods to compute the dot product, cross product, length, normalization, distance,
 * angle between two vectors, translation, rotation, and scaling.
 */
class gmath
{
public:
    // Dot product
    static double dot(const vec3 &a, const vec3 &b);
    // Cross product
    static vec3 cross(const vec3 &a, const vec3 &b);
    // Length of a vector
    static double length(const vec3 &v);
    // Normalize a vector
    static vec3 normalize(const vec3 &v);
    // Distance between two points
    static double distance(const vec3 &p1, const vec3 &p2);
    // Angle between two vectors rad
    static double angleBetween(const vec3 &a, const vec3 &b);
    // Angle between two vectors degree
    static double angleBetweenDegree(const vec3 &vec1, const vec3 &vec2);
    // Translation
    static vec3 translateVec(const vec3 &vec, const vec3 &offset);
    // Translation of a point
    static point translatePoint(const point &p, const vec3 &offset);
    // Translation of a point with a factor
    static point translatePointFactor(const point &p, const vec3 &offset, double t);
    // Rotation
    static vec3 rotate(const vec3 &v, const vec3 &axis, double angle);
    // Scaling
    static vec3 scale(const vec3 &v, const vec3 &factors);
    // Does it Intersect
    static bool intersect(const ray &p, const ray &offset);
    // Intersection location
    static point *intersectLocation(const ray &r1, const ray &r2);
    static point *intersect3d1(const ray &r1, const point arr[3]);
    static Hit *intersect3dHit(const ray &r1, const point arr[3]);
    static point *intersect3d2(const ray &r1, const point arr[4]);
    static bool intersectRaySphere(const ray &r1, const point center, const double radius);

    static vec3 *reflectorVector(const vec3 incoming, const vec3 normal);

    // project 3d triangle into 2d triangle
    static std::vector<point> projectTriangle(const point &a, const point &b, const point &c);
    // absolute value
    static double abs(double a)
    {
        return a < 0 ? -a : a;
    }

private:
    static constexpr long double epsilon = 1.0e-10; //  this is the smallest value that can be represented in the system
    /*
    This keyword specifies that epsilon is a constant expression.
    A constexpr variable is evaluated at compile time,
    which can lead to performance optimizations since the value is known and fixed during compilation.
    It also ensures that the value of epsilon cannot be modified after its initialization.
    */
    static constexpr long double pi = 3.14159265358979323846; //  this is the smallest value that can be represented in the system
};
#endif // GMATH_H