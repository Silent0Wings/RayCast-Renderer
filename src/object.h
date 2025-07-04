/**
 * @file object.h
 * @brief Defines the Object class for graphical operations.
 */
#ifndef OBJECT_H
#define OBJECT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include "point.h"
#include "color.h"
#include "vec3.h"
#include "quaternion.h"
#include "texture.h"
#include "general.h"
#include "MeshReader.h"

using namespace std;

/**
 * @class object
 * @brief Represent an obstract entity that holds the graphical data of an object.
 * The object class holds a 2D vector of pixels representing the object's graphical data.
 * and maps an array of 3 elements to a color. aka vertex color map.
 */
class object
{
public:
    point globalPosition;
    point center;
    vec3 globalRotation;
    point globallRotation;
    vec3 locallRotation;
    object *parent;
    texture tex;
    double sphereRadius;

    // 2D vector of pixels representing the object's graphical data.
    vector<vector<point>> vertices;
    // Dictionary to link an array of 3 elements to a color.
    map<array<point, 3>, color> colorMap;

    bool isEmisive = false;

    // Create an enum variable and assign a value to it

    // Constructs a new Object.
    object()
    {
        parent = nullptr;
        tex = texture();
    }
    object(const vector<vector<point>> &v)
    {
        vertices = v;
        parent = nullptr;
        tex = texture();
    }
    object(primitive prim, double scale, point offset, double angle = 0, vec3 axis = vec3(0, 0, 0))
    {
        // Initialize the object based on the primitive type
        switch (prim)
        {
        case primitive::plane:
            this->plane(scale, offset, axis, angle);
            break;
        case primitive::circle:
            this->circle(scale, offset, axis, angle);
            break;
        case primitive::cone:
            this->cone(scale, offset, axis, angle);
            break;
        case primitive::torus:
            this->torus(scale, offset, axis, angle);
            break;
        case primitive::cube:
            this->cube(scale, offset, axis, angle);
            break;
        case primitive::sphere:
            this->sphere(scale, offset, axis, angle);
            break;
        case primitive::suzane:
            this->suzane(scale, offset, axis, angle);
            break;
        default:
            throw std::invalid_argument("Unknown primitive type");
        }

        globalPosition = offset;
    }
    void cube(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        vector<point> cubeVertices = {
            (point(0, 0, 0) * scaling + offset), // Vertex 0
            (point(1, 0, 0) * scaling + offset), // Vertex 1
            (point(1, 1, 0) * scaling + offset), // Vertex 2
            (point(0, 1, 0) * scaling + offset), // Vertex 3
            (point(0, 0, 1) * scaling + offset), // Vertex 4
            (point(1, 0, 1) * scaling + offset), // Vertex 5
            (point(1, 1, 1) * scaling + offset), // Vertex 6
            (point(0, 1, 1) * scaling + offset)  // Vertex 7
        };

        if (angle == 0 || axis == vec3::zero())
        {
        }
        else
        {
            for (size_t i = 0; i < cubeVertices.size(); i++)
            {
                cubeVertices.at(i) = quaternion::rotate(cubeVertices.at(i), angle, axis);
            }
        }

        center = point(0, 0, 0);
        for (size_t i = 0; i < cubeVertices.size(); i++)
        {
            center += cubeVertices.at(i);
        }
        center /= cubeVertices.size();

        // Create object vertices for two triangles in the z = 0 plane
        const vector<vector<point>>
            v = {
                // Bottom face
                {cubeVertices[0], cubeVertices[1], cubeVertices[2]},
                {cubeVertices[0], cubeVertices[2], cubeVertices[3]},

                // Top face
                {cubeVertices[4], cubeVertices[5], cubeVertices[6]},
                {cubeVertices[4], cubeVertices[6], cubeVertices[7]},

                // Front face
                {cubeVertices[0], cubeVertices[1], cubeVertices[5]},
                {cubeVertices[0], cubeVertices[5], cubeVertices[4]},

                // Back face
                {cubeVertices[2], cubeVertices[3], cubeVertices[7]},
                {cubeVertices[2], cubeVertices[7], cubeVertices[6]},

                // Left face
                {cubeVertices[0], cubeVertices[3], cubeVertices[7]},
                {cubeVertices[0], cubeVertices[7], cubeVertices[4]},

                // Right face
                {cubeVertices[1], cubeVertices[2], cubeVertices[6]},
                {cubeVertices[1], cubeVertices[6], cubeVertices[5]}};

        colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[2]}] = color(255, 0, 0); // White
        colorMap[{cubeVertices[0], cubeVertices[2], cubeVertices[3]}] = color(255, 0, 0); // White

        // Top face

        colorMap[{cubeVertices[4], cubeVertices[5], cubeVertices[6]}] = color(255, 0, 0); // Red
        colorMap[{cubeVertices[4], cubeVertices[6], cubeVertices[7]}] = color(255, 0, 0); // Red

        // Front face
        colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[5]}] = color(0, 0, 255); // Blue
        colorMap[{cubeVertices[0], cubeVertices[5], cubeVertices[4]}] = color(0, 0, 255); // Blue

        // Back face
        colorMap[{cubeVertices[2], cubeVertices[3], cubeVertices[7]}] = color(0, 255, 255); // Cyan
        colorMap[{cubeVertices[2], cubeVertices[7], cubeVertices[6]}] = color(0, 255, 255); // Cyan

        // Left face
        colorMap[{cubeVertices[0], cubeVertices[3], cubeVertices[7]}] = color(255, 255, 0); // Yellow
        colorMap[{cubeVertices[0], cubeVertices[7], cubeVertices[4]}] = color(255, 255, 0); // Yellow

        // Right face
        colorMap[{cubeVertices[1], cubeVertices[2], cubeVertices[6]}] = color(0, 255, 0); // Green
        colorMap[{cubeVertices[1], cubeVertices[6], cubeVertices[5]}] = color(0, 255, 0); // Green

        vertices = v;

        sphereRadius = 0;
        // creating a relative sphere at with it center the center of the mesh and its radius the farthers point from that center
        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = 0; j < vertices.at(i).size(); j++)
            {
                double tempdistance = gmath::distance(center, vertices[i][j]);
                // cout << tempdistance;
                if (tempdistance > sphereRadius)
                {
                    // sphereRadius = (tempdistance > sphereRadius) ? tempdistance : sphereRadius;
                    sphereRadius = tempdistance;
                }
            }
        }
        sphereRadius *= 2;
    }

    void MoveTo(point target)
    {
        map<array<point, 3>, color> newcolorMap;
        const point origin = center;
        const vec3 translationDirection = target - origin;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            array<point, 3> triangle = {
                vertices[i][0],
                vertices[i][1],
                vertices[i][2]};

            array<point, 3> newTriangle = {
                vertices[i][0] + translationDirection,
                vertices[i][1] + translationDirection,
                vertices[i][2] + translationDirection};

            newcolorMap[newTriangle] = colorMap.at(triangle);
        }
        colorMap = newcolorMap;
    }

    void sphere(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\sphere.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void circle(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\circle.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void cone(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\cone.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void torus(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\torus.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void plane(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\plane.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void suzane(double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        loadMesh(".\\Mesh\\Suzane.txt", scaling, offset, axis, angle);
        randomColoring();
    }
    void randomColoring()
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = 0; j < vertices[i].size(); j++)
            {
                color temp;
                temp.randomColor(); // Generate random color
                colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = temp;
            }
        }
    }

    void setColor(color c)
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = 0; j < vertices[i].size(); j++)
            {
                colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = c;
            }
        }
    }
    void loadMesh(string mame, double scaling, point offset, vec3 axis = vec3(0, 0, 0), double angle = 0)
    {
        std::string filename = mame;
        MeshReader reader(filename);
        std::vector<std::vector<point>> ver;
        if (!reader.convertMesh(&ver))
        {
            std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
            return;
        }

        vector<vector<point>> meshVertices;
        size_t div = 0; // helps calculate the total number of verticies for a later use
        center = vec3(0, 0, 0);

        for (size_t i = 0; i < ver.size(); i++)
        {
            vector<point> temp;
            for (size_t j = 0; j < ver.at(i).size(); j++)
            {
                point unfilteredPosition = (ver.at(i).at(j) * scaling + offset);
                point rotatedPosition = unfilteredPosition;
                if (angle == 0 || axis == vec3::zero() || (gmath::magnitude(axis) < 1e-6))
                {
                    rotatedPosition = unfilteredPosition;
                }
                else
                {
                    rotatedPosition = quaternion::rotate(unfilteredPosition, angle, axis);
                }

                temp.push_back(rotatedPosition);
                center += rotatedPosition; // sum all the positions
                div++;                     // increment the div which is the total number of verticies
            }
            meshVertices.push_back(temp);
        }

        if (div == 0)
        {
            throw std::runtime_error("Division by zero: no vertices were processed during mesh loading.");
        }
        center /= div;
        vertices = meshVertices;

        sphereRadius = 0;
        // creating a relative sphere at with it center the center of the mesh and its radius the farthers point from that center
        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = 0; j < vertices.at(i).size(); j++)
            {
                double tempdistance = gmath::distance(center, vertices[i][j]);
                if (tempdistance > sphereRadius)
                {
                    sphereRadius = tempdistance;
                }
            }
        }
        sphereRadius *= 2;
        // std::cout << "Center: " << center << ", Radius: " << sphereRadius << std::endl;
    }

    // output operator
    friend ostream &operator<<(ostream &os, const object &obj)
    {
        os << "Object(" << obj.vertices.size() << " vertices)\n";
        os << "Center : " << obj.center << "\n";
        os << "Sphere Radius : " << obj.sphereRadius << "\n";

        for (size_t i = 0; i < obj.vertices.size(); ++i)
        {
            os << "  ";
            for (size_t j = 0; j < obj.vertices[i].size(); ++j)
            {
                os << obj.vertices[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Equality operator
    bool operator==(const object &other) const
    {
        if (vertices.size() != other.vertices.size())
        {
            return false;
        }
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            if (vertices[i].size() != other.vertices[i].size())
            {
                return false;
            }
            for (size_t j = 0; j < vertices[i].size(); ++j)
            {
                if (vertices[i][j] != other.vertices[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Inequality operator
    bool operator!=(const object &other) const
    {
        return !(*this == other);
    }
};
#endif // OBJECT_H