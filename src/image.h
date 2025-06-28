/**
 * @file image.h
 * @brief Defines the image class for image operations.
 */
#ifndef IMAGE_H
#define IMAGE_H

#include <cmath>
#include <iostream>
#include <vector>  // Import the vector library
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
class image
{
private:
    vector<vector<color>> pixels;
    unsigned int width;
    unsigned int height;

public:
    // Default constructor
    image() : image(0, 0) {}
    image(const int h, const int w)
    {

        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0)
        {
            width = 0;
            height = 0;
            pixels = vector<vector<color>>();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            pixels.resize(height);
            for (unsigned int i = 0; i < height; ++i)
            {
                pixels[i].resize(width);
            }
        }
    }
    image(const int h, const int w, vector<vector<color>> img)
    {

        if (w < 0 || h < 0)
        {
            throw std::invalid_argument("Width and height must be non-negative");
        }
        if (w == 0 && h == 0)
        {
            width = 0;
            height = 0;
            pixels = vector<vector<color>>();
        }
        else
        {
            this->width = static_cast<unsigned int>(w);
            this->height = static_cast<unsigned int>(h);
            pixels.resize(static_cast<size_t>(h));
            for (unsigned int i = 0; i < height; ++i)
            {
                // cout << i << endl ;
                pixels[i].resize(width);
            }
            for (size_t i = 0; i < pixels.size(); i++)
            {
                for (size_t j = 0; j < pixels.at(i).size(); j++)
                {
                    pixels[i][j] = img.at(i).at(j);
                }
            }
        }
    }
    image(const int h, const int w, vector<vector<image>> images) : image(w, h)
    {
        imageConstruct(images);
    }

    size_t size()
    {
        if ((width == 0 && height == 0) && pixels.size() == 0)
        {
            return 0;
        }

        if (pixels.empty() || pixels.at(0).empty() || pixels.size() * pixels.at(0).size() != width * height)
        {
            throw std::invalid_argument("size of pixels isn't the same as dimension: ");
        }
        return width * height;
    }

    // empty function
    bool empty() const
    {
        return pixels.empty();
    }

    // construct image from a vector of vectors of cameras
    void imageConstruct(const vector<vector<image>> &images)
    {
        cout << "Image Construct" << endl;
        vector<vector<color>> newpixels;

        // Calculate total height and width based on the input images
        unsigned int totalHeight = 0;
        unsigned int totalWidth = 0;
        cout << 0 << endl;

        for (const auto &row : images)
        {
            unsigned int rowHeight = 0;
            unsigned int rowWidth = 0;
            for (const auto &img : row)
            {
                rowWidth += img.getwidth();
                rowHeight = max(rowHeight, img.getheight());
            }
            totalWidth = max(totalWidth, rowWidth);
            totalHeight += rowHeight;
        }

        // Resize the newpixels vector
        cout << 1 << endl;
        newpixels.resize(totalHeight, vector<color>(totalWidth));
        cout << 2 << endl;

        // Construct the new image by placing sub-images at appropriate offsets
        unsigned int yOffset = 0;

        for (const auto &row : images)
        {
            unsigned int xOffset = 0;
            unsigned int maxRowHeight = 0;

            for (const auto &img : row)
            {
                for (size_t z = 0; z < img.getheight(); ++z)
                {
                    for (size_t w = 0; w < img.getwidth(); ++w)
                    {
                        newpixels[yOffset + z][xOffset + w] = img.get(z, w);
                    }
                }

                xOffset += img.getwidth(); // Shift X offset by the image width
                maxRowHeight = max(maxRowHeight, img.getheight());
            }

            yOffset += maxRowHeight; // Shift Y offset by the maximum row height
        }
        cout << 3 << endl;

        // Assign newpixels to the class member
        pixels = newpixels;

        cout << "pixels size = " << pixels.size() << endl;
    }

    // Get the width of the image
    unsigned int getwidth() const
    {
        return width;
    }

    // Get the height of the image
    unsigned int getheight() const
    {
        return height;
    }

    // Get all the pixels
    vector<vector<color>> getPixels() const
    {
        return pixels;
    }
    // Get the color of a pixel
    const color &get(unsigned int x, unsigned int y) const
    {
        // cout << " x :" << x << " y : "<< y << endl;
        if (constrain(x, y))
        {
            cout << "get("<< x <<" , "<< y<<")"<< endl;
            cout << "width : "<< width << " | height : "<< height << endl;
            throw std::invalid_argument("image::get():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        return pixels[x][y];
    }

    // Set the color of a pixel
    // x is the height and y is the width
    void set(unsigned int x, unsigned int y, const color &c)
    {
        if (constrain(x, y))
        {
            cout << "set("<< x <<" , "<< y<<" , "<<c<<")"<< endl;
            throw std::invalid_argument("image::set_Pixel():Pixel coordinates out of bounds. x: " + std::to_string(x) + " | y: " + std::to_string(y));
        }
        pixels[x][y] = c;
    }

    void setPixels()
    {
    }
    // Check if a pixel is out of bounds
    bool constrain(unsigned int &x, unsigned int &y) const
    {
        // std::cout << "constrain :" << std::endl;
        // std::cout << "x : " << x << " y : " << y << std::endl;
        // std::cout << "height : " << height << " width : " << width << std::endl;

        return (x >= height || y >= width || x < 0 || y < 0);
    }

    // Clear the image
    void clear()
    {
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                pixels[i][j].setblack();
            }
        }
    }

    // Resize the image
    void resize(unsigned int new_width, unsigned int new_height)
    {
        if (new_width == 0 || new_height == 0)
        {
            throw std::invalid_argument("New width and height must be non-zero");
        }
        try
        {
            pixels.resize(new_height);
            for (unsigned int i = 0; i < new_height; ++i)
            {
                pixels[i].resize(new_width);
            }
            width = new_width;
            height = new_height;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream &operator<<(std::ostream &os, const image &img)
    {
        os << "Image(" << img.getwidth() << ", " << img.getheight() << ")\n";
        for (unsigned int i = 0; i < img.getheight(); ++i)
        {
            for (unsigned int j = 0; j < img.getwidth(); ++j)
            {
                os << img.pixels[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const image &other) const
    {
        if (width != other.width || height != other.height)
        {
            return false;
        }
        for (unsigned int i = 0; i < height; ++i)
        {
            for (unsigned int j = 0; j < width; ++j)
            {
                if (pixels[i][j] != other.pixels[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Overload operator!=
    bool operator!=(const image &other) const
    {
        return !(*this == other);
    }
    // Copy constructor
    image(const image &other)
        : pixels(other.pixels), width(other.width), height(other.height) {}
};
#endif // IMAGE_H