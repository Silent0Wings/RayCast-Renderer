/**
 * @file image.h
 * @brief Defines the image class for image operations.
 */
#ifndef IMAGE_H
#define IMAGE_H

#include <cmath>
#include <iostream>
#include <vector> // Import the vector library
#include "color.h" // Assuming vec3 is implemented in vec3.h
using namespace std;

/**
 * @class image
 * @brief Represents an image in 2D space.
 * The image class encapsulates an image defined by a 2D array of pixels.
 * It provides methods to get and set these attributes, as well as to clear the image.
 * Inherits from color class.
 *  
 */
class image {
private:
    vector<vector<color>> pixels;
    unsigned int width;
    unsigned int height;
public:
    // Default constructor
    image() : image(0, 0) {}
    image(int width, int height) {

        if (width < 0 || height < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (width==0 && height == 0)
        {
            width=0;
            height=0;
            pixels=vector<vector<color>>();
        }else   
        {

            this->width = width;
            this->height = height;
            pixels.resize(height);
            for (unsigned int i = 0; i < (unsigned int) height; ++i) {
                pixels[i].resize(width);
            }
        }
    }

    // Get the width of the image
    unsigned int getwidth() const {
        return width;
    }

    // Get the height of the image
    unsigned int getheight() const {
        return height;
    }

    // Get the color of a pixel
    color get(unsigned int x, unsigned int y) const {
        if (constrain(x,y)) {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return pixels[x][y];
    }

    // Set the color of a pixel
    // x is the height and y is the width
    void set(unsigned int x, unsigned int y, const color& c) {
        if (constrain(x,y)) {
            throw std::invalid_argument("Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        pixels[x][y] = c;
    }

    // Check if a pixel is out of bounds
    bool constrain( unsigned int& x, unsigned int& y) const{
        return (x < 0 || x >= height || y < 0 || y >= width);
    }

    // Clear the image
    void clear() {
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                pixels[i][j].setblack();
            }
        }
    }

    // Resize the image
    void resize(unsigned int new_width, unsigned int new_height) {
        if (new_width < 0 || new_height < 0) {
            throw std::invalid_argument("Width and height must be non-negative width: " + std::to_string(new_width) + " | height : " + std::to_string(new_height));
            
        }
        vector<vector<color>> pixels;


    }

    // Overload operator<< for prunsigned inting
    friend std::ostream& operator<<(std::ostream& os, const image& img) {
        os << "Image(" << img.getwidth() << ", " << img.getheight() << ")\n";
        for (unsigned int i = 0; i < img.getheight(); ++i) {
            for (unsigned int j = 0; j < img.getwidth(); ++j) {
                os << img.pixels[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const image& other) const {
        if (width != other.width || height != other.height) {
            return false;
        }
        for (unsigned int i = 0; i < height; ++i) {
            for (unsigned int j = 0; j < width; ++j) {
                if (pixels[i][j] != other.pixels[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Overload operator!=
    bool operator!=(const image& other) const {
        return !(*this == other);
    }



};
#endif // IMAGE_H