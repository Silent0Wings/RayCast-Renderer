/**
 * @file RayTrace.h
 * @brief Defines the a point class for point operations.
 */
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "ray.h"
#include "Hit.h"
#include "object.h"
#include "gmath.h"
#include <vector>
#include <string>

using namespace std;

class RayTrace
{
public:
    vector<Hit> Path;
    ray SourceRay;
    // constructor
    RayTrace(const ray r)
    {
        SourceRay = r;
    }
    // trace the path and keeps track
    void trace(const size_t Bounce, const vector<object> *objects)
    {
        if (Bounce == 0 || Bounce > 100)
        {
            std::string temp = std::to_string(Bounce) + " Bounce value invalid";
            throw temp;
        }

        ray currentRay = SourceRay;

        for (size_t i = 0; i < Bounce; i++)
        {
            for (size_t j = 0; j < objects->size(); j++)
            {
                Hit currentHit = handleIntersection(objects->at(j), currentRay);
                if (!currentHit.null)
                {
                    Path.push_back(currentHit);
                    currentRay = ray(currentHit.hitPoint, currentHit.outgoing);
                }
            }
        }
    }

    Hit handleIntersection(const object obj, const ray r1)
    {
        double distance = 1.0e18;
        bool triggered = false;
        // Iterate through the color map vertices
        for (auto const &x : obj.colorMap)
        {
            // Get the vertices as an array
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            Hit *val = gmath::intersect3dHit(r1, arr.data());

            if (val != nullptr)
            {
                // Set the pixel in the image
                double leng = gmath::distance(r1.getOrigine(), val->hitPoint);
                delete val;

                // Check if the distance exceeds or equals the specified length
                if (distance >= leng)
                {
                    // Clamp the distance to the maximum length
                    distance = leng;

                    // Temporary variables for holding the color
                    color temp00 = x.second;
                    val->colorValue = temp00;

                    // Indicate that the condition was triggered
                    triggered = true;
                }
            }
        }
    }

    color getPixelValue()
    {
        color temp = color();
        for (auto const &x : Path)
        {
            temp = (temp + x.colorValue) / 2;
        }

        return temp;
    }
};

#endif // POINT_H