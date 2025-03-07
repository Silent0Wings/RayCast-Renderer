/**
 * @file vec3.h
 * @brief Defines the vector 3 class for ray tracing operations.
 */

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

/**
 * @class vec3
 * @brief Represents a 3D vector.
 * The vec3 class encapsulates a 3D vector defined by x, y, z components.
 * It provides methods to get and set these attributes, as well as to perform
 * vector addition, subtraction, multiplication, and division.
 */
class vec3
{
private:
    double components[3] = {0, 0, 0}; // Declaration

public:
    // Constructor
    vec3()
    {
        components[0] = 0;
        components[1] = 0;
        components[2] = 0;
    }
    vec3(double x, double y, double z)
    {
        components[0] = x;
        components[1] = y;
        components[2] = z;
    }
    vec3(const double val[3]) : vec3(val[0], val[1], val[2]) {}
    vec3(vec3 p, vec3 q)
    {
        vec3 result = q - p;
        components[0] = result.x();
        components[1] = result.y();
        components[2] = result.z();
    }
    vec3(std::string s)
    {
        // Check for non-empty string and ensure it is enclosed in parentheses
        if (!s.empty() && s.front() == '(' && s.back() == ')')
        {
            // Remove the enclosing parentheses
            s = s.substr(1, s.length() - 2);

            // Split the string by commas
            std::string delimiter = ",";
            size_t pos = 0;
            std::string token;
            double coords[3] = {0, 0, 0}; // Initialize x, y, z
            int i = 0;

            while ((pos = s.find(delimiter)) != std::string::npos && i < 3)
            {
                token = s.substr(0, pos);
                coords[i++] = std::stod(token); // Convert to double
                s.erase(0, pos + delimiter.length());
            }

            // Handle the last value (z-coordinate)
            if (!s.empty() && i < 3)
            {
                coords[i++] = std::stod(s);
            }

            // Assign parsed values to the coordinates
            set_x(coords[0]);
            set_y(coords[1]);
            set_z(coords[2]);
        }
        else
        {
            throw std::invalid_argument("Invalid format for vec3 string: " + s);
        }
    }

    // Copy constructor
    vec3(const vec3 &other)
    {
        for (int i = 0; i < 3; ++i)
        {
            components[i] = other.components[i];
        }
    }

    vec3(const vec3 *other)
    {
        for (int i = 0; i < 3; ++i)
        {
            components[i] = other->components[i];
        }
    }

    // Getter
    double x() const { return components[0]; }
    double y() const { return components[1]; }
    double z() const { return components[2]; }

    // Setter
    void set_x(double x) { components[0] = x; }
    void set_y(double y) { components[1] = y; }
    void set_z(double z) { components[2] = z; }
    void set(const double val[3])
    {
        set(val[0], val[1], val[2]);
    }
    void set(const double x, const double y, const double z)
    {
        components[0] = x;
        components[1] = y;
        components[2] = z;
    }

    // Clear the vector
    void clear()
    {
        set(0, 0, 0);
    }

    // Overload operator<<
    friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
    {
        os << v.toString();
        return os;
    }

    // toString method
    std::string toString() const
    {
        return "(" + std::to_string(components[0]) + ", " +
               std::to_string(components[1]) + ", " +
               std::to_string(components[2]) + ")";
    }

    // return a vector with all components set to 0
    static vec3 zero()
    {
        return vec3(0, 0, 0);
    }

    // return a vector with all components set to 1
    static vec3 one()
    {
        return vec3(1, 1, 1);
    }

    // Overload operator+
    vec3 operator+(const vec3 &other) const
    {
        return vec3(components[0] + other.components[0],
                    components[1] + other.components[1],
                    components[2] + other.components[2]);
    }

    // Overload operator-
    vec3 operator-(const vec3 &other) const
    {
        return vec3(components[0] - other.components[0],
                    components[1] - other.components[1],
                    components[2] - other.components[2]);
    }

    // Overload operator*
    vec3 operator*(double scalar) const
    {
        return vec3(components[0] * scalar,
                    components[1] * scalar,
                    components[2] * scalar);
    }

    // Overload operator/

    vec3 operator/(double scalar) const
    {
        if (scalar == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        return vec3(components[0] / scalar,
                    components[1] / scalar,
                    components[2] / scalar);
    }

    // overload operator==
    bool operator==(const vec3 &other) const
    {
        return components[0] == other.components[0] &&
               components[1] == other.components[1] &&
               components[2] == other.components[2];
    }

    // overload operator!=
    bool operator!=(const vec3 &other) const
    {
        return !(*this == other);
    }

    // overload operator+=
    vec3 &operator+=(const vec3 &other)
    {
        components[0] += other.components[0];
        components[1] += other.components[1];
        components[2] += other.components[2];
        return *this;
    }

    // overload operator-=
    vec3 &operator-=(const vec3 &other)
    {
        components[0] -= other.components[0];
        components[1] -= other.components[1];
        components[2] -= other.components[2];
        return *this;
    }

    // overload operator*=
    vec3 &operator*=(double scalar)
    {
        components[0] *= scalar;
        components[1] *= scalar;
        components[2] *= scalar;
        return *this;
    }

    // overload operator/=
    vec3 &operator/=(double scalar)
    {
        if (scalar == 0)
        {
            throw std::invalid_argument("Division by zero is not allowed");
        }
        components[0] /= scalar;
        components[1] /= scalar;
        components[2] /= scalar;
        return *this;
    }

    // overload operator=
    vec3 &operator=(const vec3 &other)
    {
        if (this != &other)
        { // Check for self-assignment
            std::copy(std::begin(other.components), std::end(other.components), std::begin(components));
        }
        return *this;
    }
};
#endif // VEC3_H