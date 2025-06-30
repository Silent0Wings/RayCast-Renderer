/**
 * @file ascii.h
 * @brief Defines the 3d character .
 */
#ifndef ASCII_H
#define ASCII_H

#include <iostream>
#include "object.h"

using namespace std;

/**
 * @class ascii
 * @brief 3d characters
 */
class ascii
{
public:
    size_t value;
    object obj;
    ascii()
    {
    }
    ascii(size_t v, double size, point offset)
    {
        value = v;
        obj = object();
        if (v == 32)
        {
            object obj(primitive::plane, 1, point::zero());
        }
        else
        {

            obj.loadMesh(".\\Mesh\\Ascii_File\\" + to_string(v) + ".txt", size, offset);
            obj.randomColoring();
            // obj.setColor(color(500));
        }
    }
};
#endif // ASCII_H