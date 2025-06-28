/**
 * @file LightReceptor.h
 * @brief Defines the LightReceptor class for LightReceptor operations.
 */
#ifndef LIGHTRECEPTOR_H
#define LIGHTRECEPTOR_H

#include <cmath>
#include <iostream>
#include <vector>   // Import the vector library
#include <array>    // Import the array library
#include "ray.h"    // Assuming vec3 is implemented in vec3.h
#include "object.h" // Assuming vec3 is implemented in vec3.h
#include "point.h"  // Ensure point is included
using namespace std;

/**
 * @class LightReceptor
 * @brief Represents an LightReceptor in 2D space.
 * The LightReceptor class encapsulates an LightReceptor defined by a 2D array of PixelReceptor.
 * It provides methods to get and set these attributes, as well as to clear the LightReceptor.
 * Inherits from color class.
 *
 */
class LightReceptor
{
private:
    vector<vector<ray>> PixelReceptor;
    unsigned int width;
    unsigned int height;
    color defaultColor;

    image img;

public:
    // Copy constructor
    LightReceptor(const LightReceptor &other)
        : PixelReceptor(other.PixelReceptor), width(other.width), height(other.height), defaultColor(other.defaultColor), img(other.img) {}
    // Default constructor
    LightReceptor() : LightReceptor(0, 0) {}
    LightReceptor(int w, int h)
    {
        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0)
        {
            width = 0;
            height = 0;
            PixelReceptor = vector<vector<ray>>();
            img = image();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            PixelReceptor.resize(height);
            for (unsigned int i = 0; i < height; ++i)
            {
                PixelReceptor[i].resize(width);
            }
            img = image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
        }
    }

    // construct the ray grid for the LightReceptor
    void consructRay(point camOrigin, point CamOffset, vec3 camDirection, unsigned int height, unsigned int width, double step)
    {
        for (unsigned i = 0; i < height; ++i)
        {
            for (unsigned j = 0; j < width; ++j)
            {
                point temp((i * step), (j * step), 0);
                PixelReceptor[i][j] = ray(camOrigin + temp + CamOffset, camDirection);
            }
        }
    }

    // Constructor with PixelReceptor
    // Constructor with PixelReceptor
    LightReceptor(int w, int h, vector<vector<ray>> g) : LightReceptor(w, h)
    {
        PixelReceptor.clear();
        PixelReceptor = g;
    }

    // Get the width of the LightReceptor
    unsigned int getwidth() const
    {
        return width;
    }

    // Get the height of the LightReceptor
    unsigned int getheight() const
    {
        return height;
    }

    // Get the image of the LightReceptor
    image getimage() const
    {
        return img;
    }

    // Get the color of a pixel
    ray get(unsigned int x, unsigned int y) const
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("LightReceptor::get():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return PixelReceptor[y][x];
    }

    // Set the color of a pixel
    void set(unsigned int x, unsigned int y, const ray &c)
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("LightReceptor::set():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        PixelReceptor[y][x] = c;
    }

    // set the ray vector
    void setRay(vector<vector<ray>> g)
    {
        PixelReceptor = g;
    }

    void setDefaultColor(const color &c)
    {
        defaultColor = c;
    }

    // Check if a pixel is out of bounds return true if out of bounds
    bool constrain(unsigned int &x, unsigned int &y) const
    {
        return (x < 0 || x >= width || y < 0 || y >= height);
    }

    // Resize the LightReceptor
    void resize(unsigned int new_width, unsigned int new_height)
    {
        if (new_width < 0 || new_height < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative width: " + std::to_string(new_width) + " | height : " + std::to_string(new_height));
        }
        vector<vector<ray>> PixelReceptor;
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream &operator<<(std::ostream &os, const LightReceptor &img)
    {
        os << "Ray( width : " << img.getwidth() << ", height : " << img.getheight() << ")\n";
        for (unsigned int i = 0; i < img.getheight(); ++i)
        {
            for (unsigned int j = 0; j < img.getwidth(); ++j)
            {
                os << img.PixelReceptor[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const LightReceptor &other) const
    {
        if (width != other.width || height != other.height)
        {
            return false;
        }
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                if (PixelReceptor[i][j] != other.PixelReceptor[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Overload operator!=
    bool operator!=(const LightReceptor &other) const
    {
        return !(*this == other);
    }
};
#endif // LIGHTRECEPTOR_H