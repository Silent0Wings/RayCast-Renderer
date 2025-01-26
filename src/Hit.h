/**
 * @file Hit .h
 * @brief Defines the Hit  class for Hit  operations.
 */

#ifndef HIT_H
#define HIT_H

#include "point.h"
#include "vec3.h"


/**
 * @class Hit 
 * @brief Represents a Hit  in 3D space.
 */ 
class Hit  {
    public:
    point hitPoint;
    vec3 normal;
    double angle;
    vec3 incoming;
    vec3 outgoing;

};
#endif // HIT_H