/**
 * @file texture.h
 * @brief Defines the texture class for texture operations.
 */
#ifndef TEXTURE_H
#define TEXTURE_H

#include <cmath>
#include <tuple>
#include <iostream>
#include <vector> // Import the vector library
#include "color.h" // Assuming vec3 is implemented in vec3.h
#include "image.h" // Assuming vec3 is implemented in vec3.h
#include "color.h" // Assuming vec3 is implemented in vec3.h
#include <map>
#include "point.h"
#include <array>

using namespace std;

/**
 * @class texture
 * @brief Represents an texture in 2D space.
 * stores an image and associate an offset and does bound checking to return a color
 *  
 */
class texture {
private:
    map<array<point, 3>,array<point, 3>> projectionMapping;
    image img;
    unsigned int xOffset;
    unsigned int yOffset;
public:
    // Default constructor
    texture(){
        // initialize the texture with 0 width and 0 height
        xOffset = 0;
        yOffset = 0;
        img = image();
        projectionMapping = map<array<point, 3>,array<point, 3>>();

    }
    texture(const int w , const int h , image newimg ,vector<array<point, 3>> vec ) {
        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("xOffset and yOffset must be non-negative");
        }else   
        {
            this->xOffset = static_cast<unsigned int> (w);
            this->yOffset  = static_cast<unsigned int> (h);
            img =newimg;
            handleProjection(vec);
        }
    }
    texture(const int w , const int h , image newimg ,vector<vector<point>> vec ) {
        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("xOffset and yOffset must be non-negative");
        }else   
        {
            this->xOffset = static_cast<unsigned int> (w);
            this->yOffset  = static_cast<unsigned int> (h);
            img =newimg;
            handleProjection(vec);
        }
    }

    void handleProjection(const std::vector<std::array<point, 3>>& vec) {
        for (const auto& x : vec) {
            point a = x.at(0);
            point b = x.at(1);
            point c = x.at(2);

            std::vector<point> test = gmath::projectTriangle(a, b, c);
        }
    }
    void handleProjection(const std::vector<vector<point>>& vec) {
        for (const auto& x : vec) {
            point a = x[0];
            point b = x[1];
            point c = x[2];
            std::vector<point> test = gmath::projectTriangle(a, b, c);
        }
    }

    // check if the texture is empty 
    bool empty() const {
        // Check if both `projectionMapping` and `img` are empty
        return projectionMapping.empty() && img.empty();
    }


    // Get the xOffset of the texture
    unsigned int getxOffset() const {
        return xOffset;
    }

    // Get the yOffset of the texture
    unsigned int getyOffset() const {
        return yOffset;
    }

    // Get the color of a pixel
    const color& get(unsigned int x, unsigned int y) const {
        //cout << " x :" << x << " y : "<< y << endl;
        //lower and upper bound correction
        x = (y<=0)? 0: ((x>=img.getheight())? img.getheight()-1: x) ;
        y = (y<=0)? 0: ((y>=img.getwidth())? img.getheight()-1: y) ;

        return img.get(x,y);
    }

 

    // Clear the texture
    void clear() {
        xOffset=0;
        yOffset=0;
        img=image();
    }


    // Overload operator<< for prunsigned inting
    friend std::ostream& operator<<(std::ostream& os, const texture& img) {
        os << "texture(" << img.getxOffset() << ", " << img.getyOffset() << ")\n";
        for (unsigned int i = 0; i < img.getyOffset(); ++i) {
            for (unsigned int j = 0; j < img.getxOffset(); ++j) {
                os << img << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const texture& other) const {
        if (xOffset != other.xOffset || yOffset != other.yOffset) {
            return false;
        }
        for (unsigned int i = 0; i < yOffset; ++i) {
            for (unsigned int j = 0; j < xOffset; ++j) {
                if (img.get(i,j) != other.get(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Overload operator!=
    bool operator!=(const texture& other) const {
        return !(*this == other);
    }


};
#endif // texture_H