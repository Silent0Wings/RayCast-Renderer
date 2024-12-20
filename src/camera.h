/**
 * @file camera.h
 * @brief Defines the camera class for camera operations.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <vector> // Import the vector library
#include "ray.h" // Assuming vec3 is implemented in vec3.h
#include "object.h" // Assuming vec3 is implemented in vec3.h
using namespace std;

/**
 * @class camera
 * @brief Represents an camera in 2D space.
 * The camera class encapsulates an camera defined by a 2D array of gridRay.
 * It provides methods to get and set these attributes, as well as to clear the camera.
 * Inherits from color class.
 *  
 */
class camera {
private:
    vector<vector<ray>> gridRay;
    unsigned int width;
    unsigned int height;
    color defaultColor;

    image img;
public:
    // Default constructor
    camera() : camera(0, 0) {}
    camera(int width, int height) {

        if (width < 0 || height < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (width==0 && height == 0)
        {
            width=0;
            height=0;
            gridRay=vector<vector<ray>>();
            img = image();
        }else   
        {
            this->width = width;
            this->height = height;
            gridRay.resize(height);
            for (unsigned int i = 0; i < (unsigned int) height; ++i) {
                gridRay[i].resize(width);
            }
            img = image(width, height);
        }
    }

    // Constructor with gridRay
    camera(int width, int height, vector<vector<ray>> g) {

        if (width < 0 || height < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        defaultColor=color(0, 0, 0);
        if (width==0 && height == 0)
        {
            width=0;
            height=0;
            gridRay=vector<vector<ray>>();
            img = image();
        }else   
        {
            this->width = width;
            this->height = height;
            gridRay.resize(height);
            for (unsigned int i = 0; i < (unsigned int) height; ++i) {
                gridRay[i].resize(width);
            }

         
            
            this->gridRay = g;
            img = image(width, height);
        }
    }

    // Get the width of the camera
    unsigned int getwidth() const {
        return width;
    }

    // Get the height of the camera
    unsigned int getheight() const {
        return height;
    }

    // Get the image of the camera
    image getimage() const {
        return img;
    }

    // Get the color of a pixel
    ray get(unsigned int x, unsigned int y) const {
        if (constrain(x,y)) {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return gridRay[y][x];
    }

    // Set the color of a pixel
    void set(unsigned int x, unsigned int y, const ray& c) {
        if (constrain(x,y)) {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        gridRay[y][x] = c;
    }

    // Check if a pixel is out of bounds return true if out of bounds
    bool constrain( unsigned int& x, unsigned int& y) const{
        return (x < 0 || x >= width || y < 0 || y >= height);
    }

    // Resize the camera
    void resize(unsigned int new_width, unsigned int new_height) {
        if (new_width < 0 || new_height < 0) {
            throw std::invalid_argument("Width and height must be non-negative width: " + std::to_string(new_width) + " | height : " + std::to_string(new_height));
        }
        vector<vector<ray>> gridRay;
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream& operator<<(std::ostream& os, const camera& img) {
        os << "Ray(" << img.getwidth() << ", " << img.getheight() << ")\n";
        for (unsigned int i = 0; i < img.getheight(); ++i) {
            for (unsigned int j = 0; j < img.getwidth(); ++j) {
                os << img.gridRay[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const camera& other) const {
        if (width != other.width || height != other.height) {
            return false;
        }
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (gridRay[i][j] != other.gridRay[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Overload operator!=
    bool operator!=(const camera& other) const {
        return !(*this == other);
    }

    // Clear the camera
    void cameraToImage(object obj) {
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                getPixel(i, j, obj, gridRay[i][j]);
            }
        }
    }

    // 
    void getPixel(unsigned int i, unsigned int j, object obj, ray r1) {
        double distance=999999999999999999;
        bool triggered = false;
        // Iterate through the color map vertices
        for (auto const& x : obj.colorMap) {
            // Get the vertices as an array
            array<point, 3> arr = {x.first[0], x.first[1], x.first[2]};

            // Check if the ray intersects with the current triangle
            point* val = gmath::intersect3d1(r1, arr.data());

            if (val != nullptr) {
                // Set the pixel in the image
                double leng = gmath::distance(r1.getOrigine(),*(val));
                delete val;
                //cout << "Length: " << leng << endl;
                //cout << "Distance: " << distance[i][j] << endl;

                if(distance >= leng)
                {
                    distance = leng;
                    // Debugging: Output the intersection and color
                    //cout << "Intersection at Pixel (" << i << ", " << j << "): " << *val << endl;
                    color temp1 = img.get(i, j);
                    img.set(i, j, x.second);
                    color temp2 = img.get(i, j);
                    if (temp1 == temp2 && temp1 != x.second)
                    {
                        throw std::invalid_argument("Pixel color not set" + std::to_string(i) + " | " + std::to_string(j));
                    }
                    triggered = true;
                    //cout << "Color: " << x.second << endl;    
                }
                //cout << "distance: " << distance[i][j] << endl;
                // Clean up memory and exit the loop (first valid intersection found)
            }
        }
    // If no intersection is found, set a default color (e.g., black)
   
    if (!triggered)
    {
        img.set(i, j, defaultColor); // Default: Black
    }
    
    // cout << "No intersection at Pixel (" << i << ", " << j << ")" << endl;
    }

   
}; 
#endif // CAMERA_H