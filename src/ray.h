/**
 * @file ray.h
 * @brief Defines the ray class for ray operations.
 * The ray class encapsulates a ray defined by an origin point and a direction vector.
 */

#ifndef RAY_H
#define RAY_H

#include <cmath>
#include "point.h" // Ensure this is included before using vec3

/**
 * @class ray
 * @brief Represents a ray in 3D space.
 * The ray class encapsulates a ray defined by an origin point and a direction vector.
 * It provides methods to get and set these attributes, as well as to compute a point at a distance t along the ray.
 */
class ray
{
private:
    point origine;
    vec3 direction;

public:
    ray() : ray(point(0, 0, 0), vec3(0, 0, 0)) {}      // Constructor
    ray(point p, vec3 v) : origine(p), direction(v) {} // Constructor

    // Getter
    point getOrigine() const { return origine; }
    vec3 getDirection() const { return direction; }

    // Setter
    void setOrigine(point p) { origine = p; }
    void setDirection(vec3 v) { direction = v; }

    // Point at a distance t along the ray
    point get(double t) const
    {
        return origine + direction * t;
    }

    // Overload operator==
    bool operator==(const ray &other) const
    {
        return (origine == other.origine && direction == other.direction);
    }

    // Overload operator!=
    bool operator!=(const ray &other) const
    {
        return !(*this == other);
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream &operator<<(std::ostream &os, const ray &r)
    {
        os << "Ray(" << r.getOrigine() << ", " << r.getDirection() << ")";
        return os;
    }
};
#endif // RAY_H