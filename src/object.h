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

using namespace std;

/**
 * @class object
 * @brief Represent an obstract entity that holds the graphical data of an object.
 * The object class holds a 2D vector of pixels representing the object's graphical data.
 * and maps an array of 3 elements to a color. aka vertex color map.
 */
class object { 
public:
    point globalPosition;
    vec3 globalRotation;
    point globallRotation;
    vec3 locallRotation;
    object* parent;

    // 2D vector of pixels representing the object's graphical data.
    vector<vector<point>> vertices;
    // Dictionary to link an array of 3 elements to a color.
    map<array<point, 3>, color> colorMap;

    // Constructs a new Object.
    object()
    {
        parent = nullptr;

    }
    object(const vector<vector<point>> v) : vertices(v) {
        parent = nullptr;
    }

    static object Cube(double scaling, point offset) {
    const vector<point> cubeVertices = {
        (point(0, 0, 0) * scaling + offset),  // Vertex 0
        (point(1, 0, 0) * scaling + offset),  // Vertex 1
        (point(1, 1, 0) * scaling + offset),  // Vertex 2
        (point(0, 1, 0) * scaling + offset),  // Vertex 3
        (point(0, 0, 1) * scaling + offset),  // Vertex 4
        (point(1, 0, 1) * scaling + offset),  // Vertex 5
        (point(1, 1, 1) * scaling + offset),  // Vertex 6
        (point(0, 1, 1) * scaling + offset)   // Vertex 7
    };
    
    // Create object vertices for two triangles in the z = 0 plane
    const vector<vector<point>> vertices = {
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
        {cubeVertices[1], cubeVertices[6], cubeVertices[5]}
        };
        return object(vertices);
    }

    // output operator
    friend ostream& operator<<(ostream& os, const object& obj) {
        os << "Object(" << obj.vertices.size() << " vertices)\n";
        for (size_t i = 0; i < obj.vertices.size(); ++i) {
            os << "  ";
            for (size_t j = 0; j < obj.vertices[i].size(); ++j) {
                os << obj.vertices[i][j] << " | ";
            }
            os << "\n";
        }
        return os;
    }

    // Equality operator
    bool operator==(const object& other) const {
        if (vertices.size() != other.vertices.size()) {
            return false;
        }
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (vertices[i].size() != other.vertices[i].size()) {
                return false;
            }
            for (size_t j = 0; j < vertices[i].size(); ++j) {
                if (vertices[i][j] != other.vertices[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // Inequality operator
    bool operator!=(const object& other) const {
        return !(*this == other);
    }
};
#endif // OBJECT_H