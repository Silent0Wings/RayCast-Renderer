/**
 * @file string_3d.h
 * @brief Defines the 3d string .
 */
#ifndef STRING_3D_H
#define STRING_3D_H

#include <iostream>
#include "object.h"
#include "ascii.h"

using namespace std;

/**
 * @class string_3d
 * @brief 3d string
 */
class string_3d
{
public:
    vector<size_t> value;
    vector<ascii> objects;
    string_3d()
    {
    }
    string_3d(string s, double spacing, double size, point offset)
    {
        if (s.length() != 0)
        {
            for (char c : s)
            {
                size_t ascii = static_cast<int>(c);
                if (ascii < 33 || ascii > 126)
                    value.push_back(32);
                else
                    value.push_back(ascii);
            }

            vec3 shitoffset = offset - vec3(8, 0, 0);

            for (size_t i = 0; i < value.size(); i++)
            {
                objects.push_back(ascii(value.at(i), size, shitoffset));
                shitoffset += vec3(spacing, 0, 0);
            }
        }
    }
};
#endif // STRING3D_H