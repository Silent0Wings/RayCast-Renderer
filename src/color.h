

#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <iostream>
#include "vec3.h" 

/**
 * @class color
 * @brief Represents a color in 3D space.
 * The color class encapsulates a color defined by red, green, blue components.
 * It provides methods to get and set these attributes, as well as to clamp the values to 0-255.
 * Inherits from vec3 class.
 **/
class color : public vec3 {
private:
    const int min=0;
    const int max=255;
public:
    // Default constructor
    color() : vec3(0, 0, 0) {}

    // Constructor with red, green, blue components
    color(double r, double g, double b) : vec3(r, g, b) {}

    void randomColor()
    {
        // randomly generate a color between 0 and 255
        set_r(rand() % 256);
        set_g(rand() % 256);
        set_b(rand() % 256);

    }

    // Getter for red component
    double r() const { return clamp(x()); }

    // Getter for green component
    double g() const { return clamp(y()); }

    // Getter for blue component
    double b() const { return clamp(z()); }

    // Setter for red component
    void set_r(double red) { set_x(clamp(red)); }

    // Setter for green component
    void set_g(double green) { set_y(clamp(green)); }

    // Setter for blue component
    void set_b(double blue) { set_z(clamp(blue)); }

    // Set color to black
    void setblack() {
        set(0, 0, 0);
    }
    
    // Set color to white
    void setwhite() {
        set(1, 1, 1);
    }

    // Set the color 
    void set(double r, double g, double b) {
        set_r(clamp(r));
        set_g(clamp(g));
        set_b(clamp(b));
    }

    // clamp values to 0-255
    double clamp(double value) const{
        if (value < min) {
            return (min);
        } else if (value > max) {
            return (max);
        }else{
            return value;
        }
    }

    // Overload operator= for setting a color
    color& operator=(const color& other) {
        set_r(other.r());
        set_g(other.g());
        set_b(other.b());
        return *this;
    }

    // Overload operator<< for prunsigned inting
    friend std::ostream& operator<<(std::ostream& os, const color& c) {
        os << "(R:" << c.r() << ", G:" << c.g() << ", B:" << c.b() << ")";
        return os;
    }

    // return an array of color values
    double* getArray() const {
        double* arr = new double[3];
        arr[0] = r();
        arr[1] = g();
        arr[2] = b();
        return arr;
    }

    // Overload operator+
    color operator+(const color& other) const {
        return color(r() + other.r(), g() + other.g(), b() + other.b());
    }
    // Overload operator/
    color operator/(double scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division by zero is not allowed");}
            return color(r() / scalar, g() / scalar, b() / scalar);
    }
};
#endif // COLOR_H