/**
 * @file LightRay .h
 * @brief Defines the LightRay  class for LightRay  operations.
 * The LightRay  class encapsulates a LightRay  defined by an origin point and a direction vector.
 */

#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include "ray.h"
#include "Hit.h"
#include "space.h"
#include <vector>


using namespace std;
/**
 * @class LightRay 
 * @brief Represents a LightRay  in 3D space.
 */ 
class LightRay : public ray  {
    public:
    unsigned int intensity;
    unsigned int maxbounces;
    vector<Hit> Hits;

    void triggerLight(space s)
    {
    }
};
#endif // LIGHTRAY_H