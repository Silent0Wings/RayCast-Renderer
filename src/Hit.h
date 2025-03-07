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

    Hit()
    {
        hitPoint = point();
        normal = vec3();
        angle = 0;
        incoming = vec3();
        outgoing = vec3();
        null = true;
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
    }
};
#endif // HIT_H