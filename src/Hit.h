/**
 * @file Hit .h
 * @brief Defines the Hit  class for Hit  operations.
 */

#ifndef HIT_H
#define HIT_H

#include "color.h"

/**
 * @class Hit
 * @brief Represents a Hit  in 3D space.
 */
class Hit
{
public:
    point hitPoint;
    vec3 normal;
    double angle;
    vec3 incoming;
    vec3 outgoing;
    bool null;
    color colorValue;
    bool ReachedLight = false;

    Hit()
    {
        hitPoint = point();
        normal = vec3();
        angle = 0;
        incoming = vec3();
        outgoing = vec3();
        null = true;
        ReachedLight = false;
    }
    Hit(point h,
        vec3 n,
        double a,
        vec3 i,
        vec3 o)
    {
        hitPoint = h;
        normal = n;
        angle = a;
        incoming = i;
        outgoing = o;
        null = false;
        ReachedLight = false;
    }

    // Copy constructor
    Hit(const Hit &other)
    {
        hitPoint = other.hitPoint;
        normal = other.normal;
        angle = other.angle;
        incoming = other.incoming;
        outgoing = other.outgoing;
        null = other.null;
        ReachedLight = other.ReachedLight;
        colorValue = other.colorValue;
    }

    Hit(const Hit *other)
    {
        hitPoint = other->hitPoint;
        normal = other->normal;
        angle = other->angle;
        incoming = other->incoming;
        outgoing = other->outgoing;
        null = other->null;
        ReachedLight = other->ReachedLight;
        colorValue = other->colorValue;
    }

    friend std::ostream &operator<<(std::ostream &os, const Hit &h)
    {
        os << std::boolalpha; // Enable textual representation for bools
        os << "Hit("
           << "hitPoint: " << h.hitPoint
           << ", normal: " << h.normal
           << ", angle: " << h.angle
           << ", incoming: " << h.incoming
           << ", outgoing: " << h.outgoing
           << ", null: " << h.null
           << ", colorValue: " << h.colorValue
           << ", ReachedLight: " << h.ReachedLight
           << ")\n";
        os << std::noboolalpha; // Reset to default representation
        return os;
    }
};
#endif // HIT_H