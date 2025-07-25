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
    camera(int h, int w, double step, point origin, vec3 indexFinger, vec3 midleFinger, int thumbFinger)
    {
        // index finger will define the row of the camera as a direction while the midle finger while define the camera width direction
        // the direction fo the ray will be calculated from these vectors this is the same concept of midle finger index and thumb orientation

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
            consructRay(height, width, step, origin, indexFinger, midleFinger, thumbFinger);
            img = image(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
        }
    }
    // Constructor with gridRay
    camera(int w, int h, vector<vector<ray>> g) : camera(w, h)
    {
        gridRay.clear();
        gridRay = g;
    }

    static camera perspectiveCamera(int h, int w, double step, point origin, vec3 indexFinger, vec3 midleFinger, int thumbFinger = 1, double perspectiveScale = 0.9, double perspectiveForce = 1)
    {

        // create a grid of rays that will be used to create the camera
        // and another grid of rays that will be used to create a difference in space between each ray
        // the  y axis in this case is depth
        // the z is left and right
        // and the x is height

        // +x moves up , +y moves left , + z move away

        // create 2 camera 1 origine and another one at the same position but much bigger
        //    camera(int h, int w, double step, point origin, vec3 indexFinger, vec3 midleFinger, int thumbFinger)
        camera camera_Origine(h, w, step, origin, indexFinger, midleFinger, thumbFinger);
        if (perspectiveScale <= 1 || perspectiveForce == 0)
        {
            return camera_Origine;
        }

        camera camera_OrigineScaled(h, w, step * perspectiveScale, origin, indexFinger, midleFinger, thumbFinger);

        // make sure ther have the same center
        point origine = camera_Origine.get(camera_Origine.getheight() / 2, camera_Origine.getwidth() / 2).getOrigine();
        point scaledOrigine = camera_OrigineScaled.get(camera_OrigineScaled.getheight() / 2, camera_OrigineScaled.getwidth() / 2).getOrigine();
        vec3 reCenteringVector = origine - scaledOrigine;
        // apply this direction to center it
        // cout << "XX1 : " << camera_Origine.get(camera_Origine.getheight() / 2, camera_Origine.getwidth() / 2).getOrigine() << " | YY1 : " << camera_OrigineScaled.get(camera_OrigineScaled.getheight() / 2, camera_OrigineScaled.getwidth() / 2).getOrigine() << endl;

        for (size_t i = 0; i < camera_OrigineScaled.getheight(); i++)
        {
            for (size_t j = 0; j < camera_OrigineScaled.getwidth(); j++)
            {
                ray currentR = camera_OrigineScaled.get(i, j);
                ray newR = ray(currentR.getOrigine() + reCenteringVector, currentR.getDirection());
                camera_OrigineScaled.set(i, j, newR);
            }
        }

        // cout << "XX2 : " << camera_Origine.get(camera_Origine.getheight() / 2, camera_Origine.getwidth() / 2).getOrigine() << " | YY2 : " << camera_OrigineScaled.get(camera_OrigineScaled.getheight() / 2, camera_OrigineScaled.getwidth() / 2).getOrigine() << endl;

        // and move the camera_OrigineScaled away from the origine camera
        for (size_t i = 0; i < camera_OrigineScaled.getheight(); i++)
        {
            for (size_t j = 0; j < camera_OrigineScaled.getwidth(); j++)
            {
                ray newR = camera_OrigineScaled.get(i, j);
                camera_OrigineScaled.set(i, j, newR.getRay(perspectiveForce));
            }
        }
        // cout << "XX3 : " << camera_Origine.get(camera_Origine.getheight() / 2, camera_Origine.getwidth() / 2).getOrigine() << " | YY3 : " << camera_OrigineScaled.get(camera_OrigineScaled.getheight() / 2, camera_OrigineScaled.getwidth() / 2).getOrigine() << endl;

        for (size_t i = 0; i < camera_Origine.getheight(); i++)
        {
            for (size_t j = 0; j < camera_Origine.getwidth(); j++)
            {
                ray originRay = camera_Origine.get(i, j);
                ray scaledRay = camera_OrigineScaled.get(i, j);
                vec3 newDirection = scaledRay.getOrigine() - originRay.getOrigine();
                newDirection = gmath::normalize(newDirection);
                ray newR(originRay.getOrigine(), newDirection);
                camera_Origine.set(i, j, newR);
            }
        }

        return camera_Origine;
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
        vec3 yd = gmath::normalize(Ydirection);
        vec3 rd = gmath::normalize(Raydirection);

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

    void consructRay(unsigned int height, unsigned int width, double step, point origin, vec3 indexFinger, vec3 midleFinger, int thumbFinger)
    {
        // vec3 xd = Xdirection = gmath::normalize(Xdirection);
        if ((int)gmath::angleBetweenDegree(midleFinger, indexFinger) != 90)
        {
            throw std::invalid_argument("Engine dosent support non proportional cameras yet : " + std::to_string((int)gmath::angleBetweenDegree(midleFinger, indexFinger)));
        }

        vec3 forward = gmath::normalize(indexFinger);
        vec3 up = gmath::normalize(midleFinger);
        vec3 direction = gmath::normalize(gmath::cross(forward, up)) * thumbFinger;

        for (size_t i = 0; i < height; i++)
        {
            point newRow = origin + (forward * step) * i;
            for (size_t z = 0; z < width; z++)
            {
                point newcolumn = newRow + (up * step) * z;
                gridRay[i][z] = ray(newcolumn, direction);
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

    // Get the grid ray 2d vector
    vector<vector<ray>> getGridRay()
    {
        return gridRay;
    }

    // Get the color of a pixel
    ray get(unsigned int x, unsigned int y) const
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("Camera::get():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return gridRay[y][x];
    }

    // Set the color of a pixel
    void set(unsigned int x, unsigned int y, const ray &r)
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument("Camera::set():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        gridRay[y][x] = r;
    }

    void setColor(unsigned int x, unsigned int y, const color &c)
    {
        if (img.constrain(x, y))
        {
            throw std::invalid_argument("Camera::setColor():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        img.set(x, y, c);
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

    // create the image that the camera would see based on the object
    void cameraToImage(object obj)
    {
        if (!obj.tex.empty() && !obj.colorMap.empty())
        {
            // cout << "No texture" << endl;
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
            // cout << "No texture" << endl;
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
            // cout << "Texture" << endl;
            for (unsigned int i = 0; i < height; ++i)
            {
                for (unsigned int j = 0; j < width; ++j)
                {
                    getPixelTexture(i, j, obj, gridRay[i][j]);
                }
            }
        }
    }

    void cameraToImageOptimized(object obj)
    {

        // cout << "No texture" << endl;
        // cout << "cameraToImageOptimized ";
        // std::cout << "height : " << height << " width : " << width << std::endl;

        // basically if the rays within the camera are not intersecting with the spehere the object is within skip them alltogether
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                //(const ray &r1, const point center, const double radius)
                if (gmath::intersectRaySphere(gridRay[i][j], obj.center, obj.sphereRadius))
                {
                    // cout << "boum" << endl;

                    getPixelColorOPtimized(i, j, obj, gridRay[i][j]);
                }
            }
        }
    }

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
            point *val = gmath::intersectRayTriangle(r1, arr.data());

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
            point *val = gmath::intersectRayTriangle(r1, arr.data());

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
            point *val = gmath::intersectRayTriangle(r1, arr.data());

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
    void getPixelColorOPtimized(unsigned int i, unsigned int j, object obj, ray r1)
    {
        double distance = 1.0e18;
        // Iterate through the color map vertices
        for (auto const &x : obj.colorMap)
        {
            // Get the vertices as an array
            // cout << "i :" << i << " | j : " << j << endl;
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            point *val = gmath::intersectRayTriangle(r1, arr.data());

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
                }
            }
        }
    }

    vector<camera> splitCamera(camera &c, size_t split)
    {
        if (split <= 1 || c.getheight() == 0 || c.getwidth() == 0)
        {
            return {camera()};
        }

        if (c.getheight() * c.getwidth() > split)
        {
            vector<ray> Linear_Ray;

            vector<vector<ray>> camera_splits;

            for (auto const &x : c.gridRay) // x is a vector<ray>
            {
                Linear_Ray.insert(Linear_Ray.end(), x.begin(), x.end());
            }

            size_t linear_size = Linear_Ray.size();
            if (linear_size >= split)
            {
                size_t chunk_size = linear_size / split;
                // cout << "chunk_size " << chunk_size << endl;
                if (chunk_size == 0)
                    return {camera()};

                vector<ray> temp;
                size_t increment = 0;
                for (size_t i = 0; i < Linear_Ray.size(); i++)
                {

                    if (increment >= chunk_size)
                    {
                        camera_splits.push_back(temp);
                        increment = 0;
                        temp.clear();
                    }

                    temp.push_back(Linear_Ray.at(i));
                    increment++;
                }

                // whatervre is remaining in the temp vector holding the values of the rays will be dumbed into the last groupe  within camera_splits
                for (auto const &x : temp) // x is a vector<ray>
                {
                    camera_splits.at(camera_splits.size() - 1).push_back(x);
                }

                if (camera_splits.size() != 0)
                {

                    vector<camera> cameras_group;
                    for (auto const &x : camera_splits)
                    {
                        camera temp = camera();
                        temp.width = x.size();
                        temp.height = 1;
                        temp.setRay({x});
                        temp.img = image(temp.height, temp.width);
                        cameras_group.push_back(temp);
                        // cout << "temp camera Height" << temp.height << " | width : " << temp.width << endl;
                    }

                    return cameras_group;
                }
            }
            else
            {
                return {camera()};
            }
        }
        else
        {
            return {camera()};
        }
        return {camera()};
    }

    image consruct_split(vector<camera> v, size_t h, size_t w)
    {

        vector<color> Linear_Ray;
        vector<vector<color>> final;

        for (auto const &cam : v) // x is a vector<ray>
        {

            vector<vector<color>> temppixel = cam.getimage().getPixels();
            if (temppixel.size() != 1)
            {
                cout << "this can only construct linear images" << endl;
                return image();
            }
            Linear_Ray.insert(Linear_Ray.end(), temppixel.at(0).begin(), temppixel.at(0).end());
        }

        size_t increment = 0;
        vector<color> temp;
        // cout << "Linear_Ray" << Linear_Ray.size() << endl;
        for (size_t i = 0; i < Linear_Ray.size(); i++)
        {

            if (increment >= w)
            {
                final.push_back(temp);
                increment = 0;
                temp.clear();
            }

            temp.push_back(Linear_Ray.at(i));
            increment++;
        }
        if (!temp.empty())
        {
            final.push_back(temp);
        }

        // cout << "height : " << h << " | width : " << w << " | final.size() : " << final.size() << " | final.at(0).size() : " << final.at(0).size() << endl;
        if (final.size() != h || final.at(0).size() != w)
            return image();
        image final_image = image(h, w, final);

        return final_image;
    }
};
#endif // CAMERA_H