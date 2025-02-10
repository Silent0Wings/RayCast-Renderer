/**
 * @file camera.h
 * @brief Defines the camera class for camera operations.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <vector>   // Import the vector library
#include <array>    // Import the array library
#include "ray.h"    // Assuming vec3 is implemented in vec3.h
#include "object.h" // Assuming vec3 is implemented in vec3.h
#include "point.h"  // Ensure point is included
using namespace std;

/**
 * @class camera
 * @brief Represents an camera in 2D space.
 * The camera class encapsulates an camera defined by a 2D array of gridRay.
 * It provides methods to get and set these attributes, as well as to clear the camera.
 * Inherits from color class.
 *
 */
class camera
{
private:
    vector<vector<ray>> gridRay;
    unsigned int width;
    unsigned int height;
    color defaultColor;
    image img;

public:
    // Copy constructor
    camera(const camera &other)
        : gridRay(other.gridRay), width(other.width), height(other.height), defaultColor(other.defaultColor), img(other.img) {}
    // Default constructor
    camera() : camera(0, 0) {}
    camera(int w, int h)
    {

        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0)
        {
            width = 0;
            height = 0;
            gridRay = vector<vector<ray>>();
            img = image();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            gridRay.resize(height);
            for (unsigned int i = 0; i < height; ++i)
            {
                gridRay[i].resize(width);
            }
            img = image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
        }
    }
    camera(const int h, const int w, const double step, const point origin, const point offset, const vec3 affect, const vec3 direction)
    {

        if (w <= 0 || h <= 0 || step <= 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0 && step == 0)
        {
            width = 0;
            height = 0;
            gridRay = vector<vector<ray>>();
            img = image();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            gridRay.resize(height);
            for (unsigned int i = 0; i < (unsigned int)height; ++i)
            {
                gridRay[i].resize(width);
            }
            consructRay(height, width, step, origin, offset, affect, direction);
            img = image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
        }
    }
    camera(int h, int w, double step, point origin, vec3 Xdirection, vec3 Ydirection, vec3 direction)
    {

        if (w <= 0 || h <= 0 || step <= 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0 && step == 0)
        {
            width = 0;
            height = 0;
            gridRay = vector<vector<ray>>();
            img = image();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            gridRay.resize(height);
            for (unsigned int i = 0; i < (unsigned int)height; ++i)
            {
                gridRay[i].resize(width);
            }
            consructRay(height, width, step, origin, Xdirection, Ydirection, direction);
            img = image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
        }
    }
    // Constructor with gridRay
    camera(int w, int h, vector<vector<ray>> g) : camera(w, h)
    {
        gridRay.clear();
        gridRay = g;
    }

    // construct the ray grid for the camera
    void consructRay(point camOrigin, point CamOffset, vec3 camDirection, unsigned int height, unsigned int width, double step)
    {
        for (unsigned i = 0; i < height; ++i)
        {
            for (unsigned j = 0; j < width; ++j)
            {
                point temp((i * step), (j * step), 0);
                gridRay[i][j] = ray(camOrigin + temp + CamOffset, camDirection);
            }
        }
    }

    // construct the ray grid for the camera
    void consructRay(unsigned int height, unsigned int width, double step, point origin, point offset, vec3 affect, vec3 direction)
    {
        affect = gmath::normalize(affect);
        for (unsigned i = 0; i < height; ++i)
        {
            for (unsigned j = 0; j < width; ++j)
            {
                point temp((i * step) * affect.x(), (j * step) * affect.y(), (j * step) * affect.z());
                gridRay[i][j] = ray(origin + offset + temp, direction);
            }
        }
    }
    void consructRay(unsigned int height, unsigned int width, double step, point origin, vec3 Xdirection, vec3 Ydirection, vec3 Raydirection)
    {
        // vec3 xd = Xdirection = gmath::normalize(Xdirection);
        vec3 yd = Ydirection = gmath::normalize(Ydirection);
        vec3 rd = Raydirection = gmath::normalize(Raydirection);

        for (unsigned i = 0; i < height; ++i)
        {
            point Yposition = point(0, 0, 0) + yd * (i * step);
            for (unsigned j = 0; j < width; ++j)
            {
                point Xposition = point(0, 0, 0) + Xdirection * (j * step);
                point Final = origin + Yposition + Xposition;
                gridRay[i][j] = ray(Final, rd);
            }
        }
    }

    // Get the width of the camera
    unsigned int getwidth() const
    {
        return width;
    }

    // Get the height of the camera
    unsigned int getheight() const
    {
        return height;
    }

    // Get the image of the camera
    image getimage() const
    {
        return img;
    }

    // Get the color of a pixel
    ray get(unsigned int x, unsigned int y) const
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return gridRay[y][x];
    }

    // Set the color of a pixel
    void set(unsigned int x, unsigned int y, const ray &c)
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        gridRay[y][x] = c;
    }

    // set the ray vector
    void setRay(vector<vector<ray>> g)
    {
        gridRay = g;
    }

    // Set the default color
    void setDefaultColor(const color &c)
    {
        defaultColor = c;
    }
    // Clear the camera
    void clear()
    {
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                img.set(i, j, defaultColor);
            }
        }
    }

    // Check if a pixel is out of bounds return true if out of bounds
    bool constrain(unsigned int &x, unsigned int &y) const
    {
        return (x < 0 || x >= width || y < 0 || y >= height);
    }

    // Resize the camera
    void resize(unsigned int new_width, unsigned int new_height)
    {
        if (new_width < 0 || new_height < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative width: " + std::to_string(new_width) + " | height : " + std::to_string(new_height));
        }
        vector<vector<ray>> gridRay;
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream &operator<<(std::ostream &os, const camera &img)
    {
        os << "Ray( width : " << img.getwidth() << ", height : " << img.getheight() << ")\n";
        for (unsigned int i = 0; i < img.getheight(); ++i)
        {
            for (unsigned int j = 0; j < img.getwidth(); ++j)
            {
                os << img.gridRay[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const camera &other) const
    {
        if (width != other.width || height != other.height)
        {
            return false;
        }
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                if (gridRay[i][j] != other.gridRay[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Overload operator!=
    bool operator!=(const camera &other) const
    {
        return !(*this == other);
    }

    // Clear the camera
    void cameraToImage(object obj)
    {
        if (!obj.tex.empty() && !obj.colorMap.empty())
        {
            cout << "No texture" << endl;
            for (unsigned int i = 0; i < height; ++i)
            {
                for (unsigned int j = 0; j < width; ++j)
                {
                    getPixelCombined(i, j, obj, gridRay[i][j]);
                }
            }
        }
        else if (obj.tex.empty())
        {
            cout << "No texture" << endl;
            for (unsigned int i = 0; i < height; ++i)
            {
                for (unsigned int j = 0; j < width; ++j)
                {
                    getPixelColor(i, j, obj, gridRay[i][j]);
                }
            }
        }
        else if (obj.colorMap.empty())
        {
            cout << "Texture" << endl;
            for (unsigned int i = 0; i < height; ++i)
            {
                for (unsigned int j = 0; j < width; ++j)
                {
                    getPixelTexture(i, j, obj, gridRay[i][j]);
                }
            }
        }
    }

    //
    void getPixelCombined(unsigned int i, unsigned int j, object obj, ray r1)
    {
        double distance = 1.0e18;
        bool triggered = false;
        // Iterate through the color map vertices
        for (auto const &x : obj.colorMap)
        {
            // Get the vertices as an array
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            point *val = gmath::intersect3d1(r1, arr.data());

            if (val != nullptr)
            {
                // Set the pixel in the image
                double leng = gmath::distance(r1.getOrigine(), *(val));
                delete val;
                // cout << "Length: " << leng << endl;
                // cout << "Distance: " << distance[i][j] << endl;

                // Check if the distance exceeds or equals the specified length
                if (distance >= leng)
                {
                    // Clamp the distance to the maximum length
                    distance = leng;

                    // Set the color at the (i, j) position in the image to the color from x.second
                    img.set(i, j, x.second);

                    // Temporary variables for blending colors
                    color temp00 = x.second;          // The color to be blended (source color)
                    color temp01 = obj.tex.get(i, j); // The existing color at the (i, j) position (destination color)

                    // Blend the colors: temp02 is the result of mixing temp00 and temp01
                    color temp02 = temp00 / 10 + temp01 / 2;

                    // Update the color at the (i, j) position with the blended result
                    img.set(i, j, temp02);

                    // Indicate that the condition was triggered
                    triggered = true;
                }

                // cout << "distance: " << distance[i][j] << endl;
                //  Clean up memory and exit the loop (first valid intersection found)
            }
        }
        // If no intersection is found, set a default color (e.g., black)

        // If no intersection is found, set a default color (e.g., black)
        if (!triggered && img.get(i, j) != defaultColor)
        {
            img.set(i, j, defaultColor); // Default: Black
        }
    }
    void getPixelTexture(unsigned int i, unsigned int j, object obj, ray r1)
    {
        double distance = 1.0e18;
        bool triggered = false;
        // Iterate through the color map vertices
        for (auto const &x : obj.colorMap)
        {
            // Get the vertices as an array
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            point *val = gmath::intersect3d1(r1, arr.data());

            if (val != nullptr)
            {
                // Set the pixel in the image
                double leng = gmath::distance(r1.getOrigine(), *(val));
                delete val;
                // cout << "Length: " << leng << endl;
                // cout << "Distance: " << distance[i][j] << endl;

                // Check if the distance exceeds or equals the specified length
                if (distance >= leng)
                {
                    // Clamp the distance to the maximum length
                    distance = leng;

                    // Set the color at the (i, j) position in the image to the color from x.second
                    img.set(i, j, x.second);

                    color temp01 = obj.tex.get(i, j);

                    img.set(i, j, temp01);

                    // Indicate that the condition was triggered
                    triggered = true;
                }

                // cout << "distance: " << distance[i][j] << endl;
                //  Clean up memory and exit the loop (first valid intersection found)
            }
        }
        // If no intersection is found, set a default color (e.g., black)
        if (!triggered && img.get(i, j) != defaultColor)
        {
            img.set(i, j, defaultColor); // Default: Black
        }

        // cout << "No intersection at Pixel (" << i << ", " << j << ")" << endl;
    }
    void getPixelColor(unsigned int i, unsigned int j, object obj, ray r1)
    {
        double distance = 1.0e18;
        bool triggered = false;
        // Iterate through the color map vertices
        for (auto const &x : obj.colorMap)
        {
            // Get the vertices as an array
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            point *val = gmath::intersect3d1(r1, arr.data());

            if (val != nullptr)
            {
                // Set the pixel in the image
                double leng = gmath::distance(r1.getOrigine(), *(val));
                delete val;

                // Check if the distance exceeds or equals the specified length
                if (distance >= leng)
                {
                    // Clamp the distance to the maximum length
                    distance = leng;

                    // Set the color at the (i, j) position in the image to the color from x.second
                    img.set(i, j, x.second);

                    // Temporary variables for blending colors
                    color temp00 = x.second;
                    img.set(i, j, temp00);

                    // Indicate that the condition was triggered
                    triggered = true;
                }
            }
        }
        // If no intersection is found, set a default color (e.g., black)
        if (!triggered && img.get(i, j) == defaultColor)
        {
            img.set(i, j, defaultColor); // Default: Black
        }
    }
};
#endif // CAMERA_H