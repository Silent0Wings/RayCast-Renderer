#include <ctime>
#include <algorithm>
#include "vec3.h"
#include "point.h"
#include "color.h"
#include "ray.h"
#include "image.h"
#include "gmath.h"
#include "gmath.cpp"
#include "camera.h"
#include "object.h"
#include "space.h"
#include "ImageRenderer.h"
#include "MeshReader.h" // Include input/output stream header
#include "texture.h"
#include "quaternion.h"
#include "perlin.h"
#include "graph.h"
#include <vector> // Include vector header
#include <future>
#include <filesystem>
using namespace std;

/**
 * @brief Tests the functionality of the vec3 class.
 *
 * This function creates several vec3 objects and performs basic vector
 * operations such as addition and subtraction. It then prints the results
 * to the standard output.
 *
 * Operations performed:
 * - Creation of vec3 objects v1 and v2
 * - Addition of v1 and v2 to create v3
 * - Subtraction of v2 from v1 to create v4
 * - Printing the vectors v1, v2, v3, and v4
 */
void testvec3()
{
    vec3 v1(1, 2, 3);
    cout << v1 << endl;
    vec3 v2(4, 5, 6);
    cout << v2 << endl;
    vec3 v3 = v1 + v2;
    cout << v3 << endl;
    vec3 v4 = v1 - v2;
    cout << v4 << endl;
}
/**
 * @brief Test function to demonstrate the usage of the color and image classes.
 *
 * This function performs the following operations:
 * - Prints a separator line.
 * - Creates two color objects with different RGB values and prints them.
 * - Prints another separator line.
 * - Creates an image object with a width and height of 3, and prints its dimensions.
 * - Prints another separator line.
 * - Sets a pixel in the image to the first color and prints the color of that pixel.
 * - Prints another separator line.
 * - Sets another pixel in the image to the second color, clears the image, and prints the color of the cleared pixel.
 * - Prints the entire image.
 */
void testimage()
{
    cout << "________________________" << endl;
    color c1(0.5, 0.5, 0.5);
    cout << c1 << endl;
    color c2(0.2, 0.3, 0.4);
    cout << c2 << endl;
    cout << "________________________" << endl;
    image img(3, 3);
    cout << img.getwidth() << endl;
    cout << img.getheight() << endl;
    cout << "________________________" << endl;
    img.set(1, 1, c1);
    cout << img.get(1, 1) << endl;
    cout << "________________________" << endl;
    img.set(2, 2, c2);
    img.clear();
    cout << img.get(2, 2) << endl;
    cout << img << endl;
}
/**
 * @brief Function to test the ray get functionality.
 *
 * This function creates a point and a vector, prints them, and then creates a ray using the point and vector.
 * It then prints the ray's points for the first two indices.
 *
 * The function performs the following steps:
 * 1. Prints a separator line.
 * 2. Creates a point p1 with coordinates (1, 2, 3) and prints it.
 * 3. Creates a vector v5 with coordinates (4, 5, 6) and prints it.
 * 4. Prints another separator line.
 * 5. Creates a ray r1 using point p1 and vector v5.
 * 6. Iterates twice, printing the ray's points at index 0 and 1, each followed by a separator line.
 */
void testrayget()
{
    cout << "________________________" << endl;
    point p1(1, 2, 3);
    cout << p1 << endl;
    vec3 v5(4, 5, 6);
    cout << v5 << endl;
    cout << "________________________" << endl;
    ray r1(p1, v5);
    for (size_t i = 0; i < 2; i++)
    {
        cout << r1.get(i) << endl;
        cout << "________________________" << endl;
    }
}
/**
 * @brief Tests the intersection of various rays.
 *
 * This function defines multiple test case rays and combines them into test cases.
 * It then iterates over each test case, checks if the rays intersect using the
 * gmath::intersect function, and outputs the result.
 *
 * Test cases include both original and additional complex rays to cover a wide range
 * of scenarios.
 *
 * The output for each test case includes:
 * - The origin and direction of the first ray.
 * - The origin and direction of the second ray.
 * - Whether the rays intersect or not.
 */
void testintersection()
{
    // Define all test case rays
    ray r4(point(0, 0, 0), vec3(1, 1, 1));     // Original case
    ray r5(point(1, 1, 1), vec3(-1, -1, -1));  // Original case
    ray r6(point(0, 0, 0), vec3(1, 0, 0));     // Original case
    ray r7(point(0, 0, 1), vec3(1, 0, 0));     // Original case
    ray r8(point(0, 0, 0), vec3(0, 1, 0));     // Original case
    ray r9(point(1, 0, 0), vec3(0, 0, 1));     // Original case
    ray r10(point(1, 1, 1), vec3(-1, -1, -1)); // Original case
    ray r11(point(0, 0, 0), vec3(0, 0, 0));    // Original case

    // Additional complex rays
    ray r12(point(1, 1, 0), vec3(0, -1, 1));         // New case
    ray r13(point(0, 0, 1), vec3(1, 1, 0));          // New case
    ray r14(point(0, 0, 0), vec3(2, 2, 2));          // New case
    ray r15(point(1, 1, 1), vec3(1, 1, 1));          // New case
    ray r16(point(10, 10, 0), vec3(-1, -1, 0));      // New case
    ray r17(point(0, 0, 0), vec3(1e9, 1e9, 1e9));    // New case
    ray r18(point(1, 1, 1), vec3(-1e9, -1e9, -1e9)); // New case
    ray r19(point(0, 0, 0), vec3(1, 0, 0));          // New case
    ray r20(point(0, 0.0001, 0), vec3(1, 0, 0));     // New case
    ray r21(point(1, 2, 0), vec3(-1, 0, 2));         // New case
    ray r22(point(0, 0, 0), vec3(1, 2, 3));          // New case
    ray r23(point(1, 0, 0), vec3(3, 2, 1));          // New case
    ray r24(point(0, 0, 0), vec3(-1, -2, -3));       // New case
    ray r25(point(1, 1, 1), vec3(2, 2, 1));          // New case

    // Combine all test cases
    vector<vector<ray>> testCases = {
        {r4, r5},   // Original case 1. Intersection: Yes
        {r4, r6},   // Original case 2. Intersection: Yes
        {r6, r7},   // Original case 3. Intersection: No
        {r6, r8},   // Original case 4. Intersection: Yes
        {r9, r10},  // Original case 5. Intersection: No
        {r10, r11}, // Original case 6. Intersection: Yes
        {r4, r8},   // Original case 7. Intersection: Yes
        {r12, r13}, // New case 8. Intersection: No
        {r14, r15}, // New case 9. Intersection: No
        {r4, r16},  // New case 10. Intersection: No
        {r17, r18}, // New case 11. Intersection: Yes
        {r19, r20}, // New case 12. Intersection: No
        {r4, r21},  // New case 13. Intersection: No
        {r22, r23}, // New case 14. Intersection: No
        {r24, r25}  // New case 15. Intersection: No
    };

    // Iterate over all test cases
    for (size_t i = 0; i < testCases.size(); ++i)
    {
        bool result = gmath::intersect(testCases[i][0], testCases[i][1]);

        // Output the result
        std::cout << "Test case " << i + 1 << ":\n";
        std::cout << "  Ray 1: Origin " << testCases[i][0].getOrigine() << ", Direction " << testCases[i][0].getDirection() << "\n";
        std::cout << "  Ray 2: Origin " << testCases[i][1].getOrigine() << ", Direction " << testCases[i][1].getDirection() << "\n";
        std::cout << "  Do the rays intersect? " << (result ? "Yes" : "No") << "\n";
        std::cout << "--------------------------\n";
    }
}
/**
 * @brief Tests various functionalities of the camera class.
 *
 * This function performs the following tests:
 * 1. Creates a camera object and prints its width and height.
 * 2. Sets the color of a pixel using a ray and prints the color of the pixel.
 * 3. Checks if a pixel is out of bounds and prints the result.
 * 4. Resizes the camera and prints the new width and height.
 */
void testCamera()
{
    cout << "_________Camera Test_______________" << endl;

    // Create a camera
    camera cam(3, 3);
    cout << cam.getwidth() << endl;
    cout << cam.getheight() << endl;
    cout << "________________________" << endl;

    // Set the color of a pixel
    ray r1(point(0, 0, 0), vec3(1, 1, 1));
    cam.set(1, 1, r1);
    cout << cam.get(1, 1) << endl;
    cout << "________________________" << endl;

    // Check if a pixel is out of bounds
    unsigned int x = 0;
    unsigned int y = 0;
    cout << std::boolalpha << cam.constrain(x, y) << endl;
    x = 4;
    y = 4;
    cout << std::boolalpha << cam.constrain(x, y) << endl;
    cout << "________________________" << endl;

    // Resize the camera
    cam.resize(4, 4);
    cout << cam.getwidth() << endl;
    cout << cam.getheight() << endl;
    cout << "________________________" << endl;
}
/**
 * @brief Tests the intersection of a ray with a set of triangles and outputs the intersection point and color.
 *
 * This function creates an object with vertices for two triangles and assigns colors to them.
 * It then creates a ray that points towards the second triangle and checks for intersection.
 * If an intersection is found, it outputs the intersection point and the color of the intersected triangle.
 *
 * The function performs the following steps:
 * 1. Creates an object with vertices for two triangles.
 * 2. Sets the color map for both triangles.
 * 3. Retrieves the second group of vertices as an array.
 * 4. Creates a ray that points towards the second triangle.
 * 5. Checks if the ray intersects with the color map and returns the color of the intersection.
 * 6. Outputs the intersection point and color if an intersection is found.
 * 7. Cleans up dynamically allocated memory.
 */
void testSpaceCamera()
{
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size
    unsigned int size = 1000;
    double step = 0.01;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i)
    {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j)
        {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 1);
            vec3 direction(0, 0, -1);
            row.push_back(ray(origin, direction));

            // Debug: Output ray origin and direction
            // std::cout << "Ray Origin: " << origin << " Direction: " << direction << std::endl;
        }
        gridRay.push_back(row);
    }
    // Create a camera with the grid of rays
    camera cam(static_cast<int>(size), static_cast<int>(size), gridRay);

    // Create object vertices for multiple triangles in the z = 0 plane
    std::vector<std::vector<point>> vertices = {
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},       // Triangle 1
        {point(1, 0, 0), point(1, 1, 0), point(0, 1, 0)},       // Triangle 2
        {point(1, 2, 0), point(1, 1, 2), point(0, 1, 0)},       // Triangle 3
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},       // Triangle 4
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},       // Triangle 5
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},       // Triangle 6
        {point(2, 2, 0), point(3, 2, 0), point(2, 3, 0)},       // Triangle 7
        {point(3, 2, 0), point(3, 3, 0), point(2, 3, 0)},       // Triangle 8
        {point(4, 4, 0), point(5, 4, 0), point(4, 5, 0)},       // Triangle 9
        {point(5, 4, 0), point(5, 5, 0), point(4, 5, 0)},       // Triangle 10
        {point(6, 6, 0), point(7, 6, 0), point(6, 7, 0)},       // Triangle 11
        {point(7, 6, 0), point(7, 7, 0), point(6, 7, 0)},       // Triangle 12
        {point(8, 8, 0), point(9, 8, 0), point(8, 9, 0)},       // Triangle 13
        {point(9, 8, 0), point(9, 9, 0), point(8, 9, 0)},       // Triangle 14
        {point(10, 10, 0), point(11, 10, 0), point(10, 11, 0)}, // Triangle 15
        {point(11, 10, 0), point(11, 11, 0), point(10, 11, 0)}, // Triangle 16
        {point(12, 12, 0), point(13, 12, 0), point(12, 13, 0)}, // Triangle 17
        {point(13, 12, 0), point(13, 13, 0), point(12, 13, 0)}, // Triangle 18
        {point(14, 14, 0), point(15, 14, 0), point(14, 15, 0)}, // Triangle 19
        {point(15, 14, 0), point(15, 15, 0), point(14, 15, 0)}, // Triangle 20
        {point(16, 16, 0), point(17, 16, 0), point(16, 17, 0)}, // Triangle 21
        {point(17, 16, 0), point(17, 17, 0), point(16, 17, 0)}, // Triangle 22
        {point(18, 18, 0), point(19, 18, 0), point(18, 19, 0)}, // Triangle 23
        {point(19, 18, 0), point(19, 19, 0), point(18, 19, 0)}, // Triangle 24
        {point(20, 20, 0), point(21, 20, 0), point(20, 21, 0)}, // Triangle 25
        {point(21, 20, 0), point(21, 21, 0), point(20, 21, 0)}, // Triangle 26
        {point(22, 22, 0), point(23, 22, 0), point(22, 23, 0)}, // Triangle 27
        {point(23, 22, 0), point(23, 23, 0), point(22, 23, 0)}, // Triangle 28
        {point(24, 24, 0), point(25, 24, 0), point(24, 25, 0)}, // Triangle 29
        {point(25, 24, 0), point(25, 25, 0), point(24, 25, 0)}  // Triangle 30
    };

    object obj(vertices);
    // iterate threw verticies
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        // Create a new object with vertices
        // Assign color map for each triangle
        if (i % 2 == 0)
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(255 / (i + 1), 0, 0); // Red
        }
        else if (i % 3 == 0)
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(0, 255 / (i + 1), 0); // Green
        }
        else
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(255 / (i + 1), 255 / (i + 1), 255 / (i + 1)); // Blue
        }
    }

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();

    std::cout << "________________________" << std::endl;
}
/**
 * @brief Tests the space, camera, and cube object setup.
 *
 * This function sets up a 3D space with a camera and a cube object. It creates a grid of rays
 * pointing towards the plane z = 0, initializes a camera with these rays, and defines a cube
 * with vertices and colored faces. The cube is then added to the space, and the camera rays
 * are triggered to interact with the cube.
 *
 * The cube is scaled and offset in the space, and each face of the cube is assigned a specific color:
 * - Bottom face: White
 * - Top face: Red
 * - Front face: Blue
 * - Back face: Cyan
 * - Left face: Yellow
 * - Right face: Green
 *
 * The function outputs debug information to the console and demonstrates the interaction
 * between the camera rays and the cube object in the defined space.
 */
void testSpaceCameraCube()
{
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size
    unsigned int size = 500 * 2;
    double step = 0.01 / 2;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i)
    {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j)
        {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 6);
            vec3 direction(0.5, 0.5, -3);
            row.push_back(ray(origin, direction));

            // Debug: Output ray origin and direction
            // std::cout << "Ray Origin: " << origin << " Direction: " << direction << std::endl;
        }
        gridRay.push_back(row);
    }

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling = 3;
    point offset = point(2, 1, 1);
    // Create object vertices for a cube
    std::vector<point> cubeVertices = {
        (point(0, 0, 0) * scaling + offset), // Vertex 0
        (point(1, 0, 0) * scaling + offset), // Vertex 1
        (point(1, 1, 0) * scaling + offset), // Vertex 2
        (point(0, 1, 0) * scaling + offset), // Vertex 3
        (point(0, 0, 1) * scaling + offset), // Vertex 4
        (point(1, 0, 1) * scaling + offset), // Vertex 5
        (point(1, 1, 1) * scaling + offset), // Vertex 6
        (point(0, 1, 1) * scaling + offset)  // Vertex 7
    };

    // Create object vertices for two triangles in the z = 0 plane
    std::vector<std::vector<point>> vertices = {
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

    // Bottom face
    object obj(vertices);

    // Bottom face
    obj.colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[2]}] = color(255, 0, 0); // White
    obj.colorMap[{cubeVertices[0], cubeVertices[2], cubeVertices[3]}] = color(255, 0, 0); // White

    // Top face
    obj.colorMap[{cubeVertices[4], cubeVertices[5], cubeVertices[6]}] = color(255, 0, 0); // Red
    obj.colorMap[{cubeVertices[4], cubeVertices[6], cubeVertices[7]}] = color(255, 0, 0); // Red

    // Front face
    obj.colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[5]}] = color(0, 0, 255); // Blue
    obj.colorMap[{cubeVertices[0], cubeVertices[5], cubeVertices[4]}] = color(0, 0, 255); // Blue

    // Back face
    obj.colorMap[{cubeVertices[2], cubeVertices[3], cubeVertices[7]}] = color(0, 255, 255); // Cyan
    obj.colorMap[{cubeVertices[2], cubeVertices[7], cubeVertices[6]}] = color(0, 255, 255); // Cyan

    // Left face
    obj.colorMap[{cubeVertices[0], cubeVertices[3], cubeVertices[7]}] = color(255, 255, 0); // Yellow
    obj.colorMap[{cubeVertices[0], cubeVertices[7], cubeVertices[4]}] = color(255, 255, 0); // Yellow

    // Right face
    obj.colorMap[{cubeVertices[1], cubeVertices[2], cubeVertices[6]}] = color(0, 255, 0); // Green
    obj.colorMap[{cubeVertices[1], cubeVertices[6], cubeVertices[5]}] = color(0, 255, 0); // Green

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();

    std::cout << "________________________" << std::endl;
}

void testSpaceCameraCube1()
{
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size
    unsigned int size = 500;
    double step = 0.01;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));

    // Define the camera origin and offset
    point camOrigin(0, 0, 0);
    point CamOffset(3.5, 3.5, 4.6); // Simplified to combine 0.5+3, 0.5+3, 1.6+3
    vec3 camDirection(-1, -1, -1);  // Pointing downward

    // Generate rays
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            // Compute the origin of the ray in the grid
            point temp(i * step, j * step, 0);
            // Create a ray at this position pointing in the camera direction
            gridRay[i][j] = ray(camOrigin + temp + CamOffset, gmath::normalize(camDirection));
        }
    }

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling = 1;
    point offset = point(0, 0, 0);
    // Create object vertices for a cube
    std::vector<point> cubeVertices = {
        (point(0, 0, 0) * scaling + offset), // Vertex 0
        (point(1, 0, 0) * scaling + offset), // Vertex 1
        (point(1, 1, 0) * scaling + offset), // Vertex 2
        (point(0, 1, 0) * scaling + offset), // Vertex 3
        (point(0, 0, 1) * scaling + offset), // Vertex 4
        (point(1, 0, 1) * scaling + offset), // Vertex 5
        (point(1, 1, 1) * scaling + offset), // Vertex 6
        (point(0, 1, 1) * scaling + offset)  // Vertex 7
    };

    // Create object vertices for two triangles in the z = 0 plane
    std::vector<std::vector<point>> vertices = {
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

    // Bottom face
    object obj(vertices);

    // Bottom face
    obj.colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[2]}] = color(255, 0, 0); // White
    obj.colorMap[{cubeVertices[0], cubeVertices[2], cubeVertices[3]}] = color(255, 0, 0); // White

    // Top face
    obj.colorMap[{cubeVertices[4], cubeVertices[5], cubeVertices[6]}] = color(255, 0, 0); // Red
    obj.colorMap[{cubeVertices[4], cubeVertices[6], cubeVertices[7]}] = color(255, 0, 0); // Red

    // Front face
    obj.colorMap[{cubeVertices[0], cubeVertices[1], cubeVertices[5]}] = color(0, 0, 255); // Blue
    obj.colorMap[{cubeVertices[0], cubeVertices[5], cubeVertices[4]}] = color(0, 0, 255); // Blue

    // Back face
    obj.colorMap[{cubeVertices[2], cubeVertices[3], cubeVertices[7]}] = color(0, 255, 255); // Cyan
    obj.colorMap[{cubeVertices[2], cubeVertices[7], cubeVertices[6]}] = color(0, 255, 255); // Cyan

    // Left face
    obj.colorMap[{cubeVertices[0], cubeVertices[3], cubeVertices[7]}] = color(255, 255, 0); // Yellow
    obj.colorMap[{cubeVertices[0], cubeVertices[7], cubeVertices[4]}] = color(255, 255, 0); // Yellow

    // Right face
    obj.colorMap[{cubeVertices[1], cubeVertices[2], cubeVertices[6]}] = color(0, 255, 0); // Green
    obj.colorMap[{cubeVertices[1], cubeVertices[6], cubeVertices[5]}] = color(0, 255, 0); // Green

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();

    std::cout << "________________________" << std::endl;
}

void testSpaceCameraCube2()
{
    std::cout << "________________________" << std::endl;
    // Define the grid size
    unsigned int size = 500 * 2;
    double step = 0.01 / 2;

    // Create a camera with the grid of rays
    point origin(2, 2, 2);
    vec3 indexFinger(1, -1, 0);
    vec3 midleFinger(-1, -1, 1);

    // index finger will define the row of the camera as a direction while the midle finger while define the camera width direction
    // the direction fo the ray will be calculated from these vectors this is the same concept of midle finger index and thumb orientation
    camera cam = camera(size, size, step, origin, midleFinger, indexFinger, -1);
    double scaling = 1.5;
    point offset = point(0, -3, 0);
    point offset1 = point(0, -1, 0);
    point offset2 = point(0, -4, 0);
    point offset3 = point(0, -4, 0);
    object cube = object(primitive::cube, scaling, offset);
    object cube1 = object(primitive::cube, scaling / 2, offset1);
    object cube2 = object(primitive::cube, scaling / 2, offset2);
    object cube3 = object(primitive::cube, scaling / 2, offset3 + vec3(1, 0, 0));

    // Create a space and assign the object
    space s({cube, cube1, cube2, cube3});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();

    std::cout << "________________________" << std::endl;
}

void testSpaceCameraCube3()
{
    // loads info from a file in which each line containes location scale and rotation of a groupe of objects

    std::cout << "_________FileLoad and conversion Test_______________" << std::endl;

    string filename = ".\\Experimental\\LocationScaleRotation2.txt";
    MeshReader reader(filename);
    vector<vector<point>> vertices;
    reader.convertMesh(&vertices);

    std::cout << "________________________" << std::endl;
    // Define the grid size
    unsigned int size = 500 * 2;
    double step = 0.01 / 2;

    // Create a camera with the grid of rays
    point origin(2, 2, 2);
    vec3 indexFinger(1, -1, 0);
    vec3 midleFinger(-1, -1, 1);

    // index finger will define the row of the camera as a direction while the midle finger while define the camera width direction
    // the direction fo the ray will be calculated from these vectors this is the same concept of midle finger index and thumb orientation
    camera cam = camera(size, size, step, origin, midleFinger, indexFinger, -1);
    double scaling = 1;
    point offset = point(0, -3, 0);

    vector<object> tempVecObject;
    // Create a space and assign the object
    // vertices x cordinate is location , y is the scale , z is the rotation
    // x is 0 , y is 1 , z is 2
    for (size_t i = 0; i < vertices.size(); i++)
    {
        double tempObjectScale = (vertices.at(i).at(1).x() + vertices.at(i).at(1).y() + vertices.at(i).at(1).z()) / 3;
        object tempObject(primitive::cube, tempObjectScale * scaling, offset + vertices.at(i).at(0));
        // cout << tempObject;
        tempVecObject.push_back(tempObject);
    }

    space s(tempVecObject);

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();

    std::cout << "________________________" << std::endl;
}

/**
 * @brief Test function to demonstrate the creation and manipulation of a space object.
 *
 * This function performs the following operations:
 * - Prints a header message to the console.
 * - Creates a space object.
 * - Adds a camera to the space.
 * - Creates a new object with specified vertices.
 * - Assigns the object to the space.
 * - Sets the color map for the object's faces.
 * - Sets the global position of the object.
 * - Sets the global rotation of the object.
 * - Sets the local rotation of the object.
 * - Sets the parent of the object to nullptr.
 * - Outputs the object to the console.
 * - Outputs the space to the console.
 * - Prints a footer message to the console.
 */
void testSpace()
{
    std::cout << "_________Object Test_______________" << std::endl;
    space space;
    // add a camera to the space
    camera cam(3, 3);

    space.cameras.push_back(cam);
    // Create a new object with vertices
    vector<vector<point>> vertices = {
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},
        {point(1, 0, 0), point(1, 1, 0), point(0, 1, 0)}};
    object obj(vertices);
    // assign the object to the space
    space.obj.push_back(obj);
    // set color map
    obj.colorMap[{point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)}] = color(255, 0, 0);
    obj.colorMap[{point(1, 0, 0), point(1, 1, 0), point(0, 1, 0)}] = color(0, 255, 0);
    // Set the global position
    obj.globalPosition = point(1, 2, 3);
    // Set the global rotation
    obj.globalRotation = vec3(4, 5, 6);
    // Set the local rotation
    obj.locallRotation = vec3(7, 8, 9);
    // Set the parent object
    obj.parent = nullptr;
    // output the object
    std::cout << obj << std::endl;
    // output the space
    std::cout << space << std::endl;
    std::cout << "________________________" << std::endl;
}
// testing the ability to  load the files from text
void testFileLoad()
{
    std::cout << "_________FileLoad and conversion Test_______________" << std::endl;

    string filename = "face_coordinates.txt";
    MeshReader reader(filename);
    vector<vector<point>> vertices;
    reader.convertMesh(&vertices);

    // print the verticies string
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            // cout << reader.verticesString[i][j] << " ";
        }
        cout << endl;
    }

    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            cout << vertices[i][j] << " ";
        }
        cout << endl;
    }

    std::cout << "________________________" << std::endl;
}
// testing the ability to load a mesh file and color the faces at high resolution
void testMeshImportAndColoringSuzane()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Suzane.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices))
    {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size = 500 * 4;
    double step = 0.006 / 4;

    size = 100;
    step = 0.01;

    // declare for this     camera(int  height, int width , int step, point origin ,vec3 Xdirection, vec3 Ydirection, vec3 direction) {
    camera cam(size, size, step, point(0, 0, 0), vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, -1));

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset(-1.5, -20, -1.5);
    vec3 camDirection(0, 1, 0); // Pointing downward
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            point temp((i * step), 0, (j * step));
            gridRay[i][j] = ray(camOrigin + temp + CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = temp;
        }
    }

    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            // cout << vertices[i][j] << " ";
        }
        // cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    // for (const auto& entry : obj.colorMap) {
    // const auto& vertex = entry.first;
    // const auto& col = entry.second;
    // std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
    //}

    std::cout << "________________________" << std::endl;

    // Create a camera with the grid of rays

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();
}
// testing the ability to load a mesh file and color the faces
void testMeshImportAndColoringDear()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Dear.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices))
    {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.02;

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset(-3, -4, -4);
    vec3 camDirection(1, 0, 0); // Pointing downward
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            point temp(0, (i * step), (j * step));
            gridRay[i][j] = ray(camOrigin + temp + CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = temp;
        }
    }

    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            // cout << vertices[i][j] << " ";
        }
        // cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    // for (const auto& entry : obj.colorMap) {
    // const auto& vertex = entry.first;
    // const auto& col = entry.second;
    // std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
    //}

    std::cout << "________________________" << std::endl;

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
}
// testing the ability to load a mesh file and color the faces
void testMeshImportAndColoringDhalia()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Dhalia.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices))
    {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.0025;

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset(-0.5, -0.5, 10);
    vec3 camDirection(0, 0, -1); // Pointing downward
    for (unsigned i = 0; i < size; ++i)
    {
        for (unsigned j = 0; j < size; ++j)
        {
            point temp((i * step), (j * step), 0);
            gridRay[i][j] = ray(camOrigin + temp + CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = temp;
        }
    }

    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            // cout << vertices[i][j] << " ";
        }
        // cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    // for (const auto& entry : obj.colorMap) {
    // const auto& vertex = entry.first;
    // const auto& col = entry.second;
    // std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
    //}

    std::cout << "________________________" << std::endl;

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
}
// testing the split rays function to split a camera into 4 cameras
void split_rays()
{

    // Define the grid size and step
    unsigned int size = 4;
    double step = 0.1;
    unsigned int height = size;
    unsigned int width = size;

    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / (num_cameraX);
    unsigned int camera_width = width / (num_cameraY);
    vector<vector<camera>> cameras;
    for (size_t i = 0; i < (num_cameraX); i++)
    {
        vector<camera> temp;
        for (size_t i = 0; i < (num_cameraY); i++)
        {
            temp.push_back(camera(camera_height, camera_width));
        }
        cout << endl;

        cameras.push_back(temp);
        cout << endl;
    }

    point camOrigin(0, 0, 0);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(1, 0, 0); // pointing right
    vec3 rayDirection(0, 0, -1); // Pointing downward

    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;
            cout << "Offset Position: " << offsetPos << endl;

            cameras[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
            cout << "Camera " << i << " " << j << " " << cameras[i][j] << endl;
        }
    }
}
// testing the split rays function with threads
void split_raysThreads()
{
    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.01;
    unsigned int height = size;
    unsigned int width = size;

    // Camera grid dimensions
    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / num_cameraX;
    unsigned int camera_width = width / num_cameraY;

    // Initialize cameras grid
    vector<vector<camera>> camerasGrid(num_cameraX, vector<camera>(num_cameraY));

    point camOrigin(0, -2, -2);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(-1, 0, 0); // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;

            camerasGrid[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
        }
    }

    std::cout << "_________File Load and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Suzane.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;

    if (!reader.convertMesh(&vertices))
    {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Assign random colors to object vertices
    for (const auto &face : vertices)
    {
        color randomColor;
        randomColor.randomColor(); // Generate random color
        obj.colorMap[{face[0], face[1], face[2]}] = randomColor;
    }

    std::cout << "________________________" << std::endl;

    // Create space and add object
    space s({obj});

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty() || s.obj.empty())
    {
        std::cout << "No cameras or objects to process." << std::endl;
        return;
    }

    std::vector<std::future<void>> futures;
    s.threadedCameraRay(futures);

    // Wait for all threads to complete
    for (auto &future : futures)
    {
        future.get();
    }

    std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

    std::cout << "_________Stitching_______________" << std::endl;

    // Stitch images into a single output
    std::cout << "Combined Width: " << combinedWidth << std::endl;
    std::cout << "Combined Height: " << combinedHeight << std::endl;

    vector<vector<image>> images; // Placeholder for stitched images

    for (int i = static_cast<int>(num_cameraX) - 1; i >= 0; i--)
    { // Outer loop iterates backward
        vector<image> rowImages;
        for (int j = static_cast<int>(num_cameraY) - 1; j >= 0; j--)
        { // Outer loop iterates backward
            rowImages.push_back(s.cameras[i * num_cameraY + j].getimage());
        }
        images.push_back(rowImages);
    }

    // swap the first and last image
    image tempimg = images[0][0];
    images[0][0] = images[num_cameraX - 1][num_cameraY - 1];
    images[num_cameraX - 1][num_cameraY - 1] = tempimg;

    image stitchedImage(combinedWidth, combinedHeight, images);
    ImageRenderer::renderToFile(stitchedImage, "stitched.ppm");
}
// testing texture projection and mergin with vertex color on a cube
void cubeTextureTest()
{
    // File paths
    string filePathOriginal = "leopard-515509.jpg";
    string filePath = "leopard.ppm";

    // Variables
    int width, height;
    vector<vector<color>> pixels;
    image img;
    texture Leo;

    cout << "read texture..." << endl;
    // Read the image
    try
    {
        pixels = ImageRenderer::readPPM(filePath, width, height);
        cout << "Image size: " << width << "x" << height << endl;
        img = image(width, height, pixels);

        // Access individual pixels for verification
        color pixel = pixels[0][0]; // Top-left pixel
        cout << "Top-left pixel: R=" << pixel.r()
             << ", G=" << pixel.g()
             << ", B=" << pixel.b() << endl;
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;
    cout << "create camera..." << endl;

    // Define the grid size
    size_t multiplier = 12;
    unsigned int size = 20 * multiplier;
    double step = 0.17 / static_cast<double>(multiplier);
    vec3 OriginOffset = vec3(.5, .5, 0);
    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i)
    {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j)
        {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 6);
            vec3 direction(0.5, 0.5, -3);
            row.push_back(ray(origin + OriginOffset, direction));
        }
        gridRay.push_back(row);
    }

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);
    cam.setDefaultColor(color(255, 255, 255));
    double scaling = .5;
    point offset = point(2.3, 2.3, 2);
    object obj(primitive::cube, scaling, offset);
    object obj1(primitive::torus, scaling, offset + point(1, 0.5, .5));
    object obj2(primitive::torus, scaling / 2, offset + point(.5, 1, .5));
    object obj3(primitive::suzane, scaling / 2, offset + point(-1, 0.5, 1));

    // Create a space and assign the object
    cout << "Setting up texture..." << endl;
    Leo = texture(width, height, img, obj.vertices); // assign texture to object
    // obj.tex = Leo;

    space s({obj, obj1, obj2, obj3});

    cout << "add camera to space..." << endl;
    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    cout << "Ray cast..." << endl;
    s.triggerCameraRay();
    cout << "Save image..." << endl;
    s.saveImages();
}
// output all the primitives hard coded in the code
void primitiveThreadTest()
{
    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.01;
    unsigned int height = size;
    unsigned int width = size;

    // Camera grid dimensions
    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / num_cameraX;
    unsigned int camera_width = width / num_cameraY;

    // Initialize cameras grid
    vector<vector<camera>> camerasGrid(num_cameraX, vector<camera>(num_cameraY));

    point camOrigin(5, 5, 5);
    vec3 camYDirection(0, 0, 1);   // Pointing upward
    vec3 camXDirection(0, 1, 0);   // Pointing right
    vec3 rayDirection(-1, -1, -1); // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;

            camerasGrid[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
        }
    }

    std::cout << "_________File Load and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Suzane.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;

    if (!reader.convertMesh(&vertices))
    {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Assign random colors to object vertices
    for (const auto &face : vertices)
    {
        color randomColor;
        randomColor.randomColor(); // Generate random color
        obj.colorMap[{face[0], face[1], face[2]}] = randomColor;
    }

    std::cout << "________________________" << std::endl;

    // Create space and add object
    space s = space();

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty())
    {
        std::cout << "No cameras or objects to process." << std::endl;
        return;
    }

    double scaling = .8;
    point offset = point(-1.25, 1.25, 0);
    object obj0(primitive::cube, scaling, offset);
    object obj1(primitive::torus, scaling, offset);
    object obj2(primitive::sphere, scaling, offset);
    object obj3(primitive::circle, scaling, offset);
    object obj4(primitive::cone, scaling, offset);
    object obj5(primitive::plane, scaling, offset);
    object obj6(primitive::suzane, scaling, offset);

    vector<object> objectBuffer = {obj0, obj1, obj2, obj3, obj4, obj5, obj6};
    // objectBuffer.size()
    //  render each object in a different thread that will be processed by the same camera
    for (size_t i = 0; i < objectBuffer.size(); i++)
    {
        s.obj.clear();
        for (size_t i = 0; i < s.cameras.size(); i++)
        {
            s.cameras[i].clear();
        }
        s.obj.push_back(objectBuffer[i]);

        std::vector<std::future<void>> futures;
        s.threadedCameraRay(futures);

        // Wait for all threads to complete
        for (auto &future : futures)
        {
            future.get();
        }

        std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

        std::cout << "_________Stitching_______________" << std::endl;

        // Stitch images into a single output
        std::cout << "Combined Width: " << combinedWidth << std::endl;
        std::cout << "Combined Height: " << combinedHeight << std::endl;

        vector<vector<image>> images; // Placeholder for stitched images

        for (int i = static_cast<int>(num_cameraX) - 1; i >= 0; i--)
        { // Outer loop iterates backward
            vector<image> rowImages;
            for (int j = static_cast<int>(num_cameraY) - 1; j >= 0; j--)
            { // Outer loop iterates backward
                rowImages.push_back(s.cameras[i * num_cameraY + j].getimage());
            }
            images.push_back(rowImages);
        }

        // swap the first and last image
        image tempimg = images[0][0];
        images[0][0] = images[num_cameraX - 1][num_cameraY - 1];
        images[num_cameraX - 1][num_cameraY - 1] = tempimg;

        image stitchedImage(combinedWidth, combinedHeight, images);
        ImageRenderer::renderToFile(stitchedImage, "primitiveThreadTest" + std::to_string(i) + ".ppm");
    }
}
// tests the camera rotation behavior
void cameraConfigTest()
{
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size = 800;
    double step = 0.006;

    point camOrigin(0, 0, 0);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(0, 0, 0);  // Pointing downward

    vec3 topRight = vec3(1, 1, 0);       // Pointing topRight
    vec3 topLeft = vec3(-1, 1, 0);       // Pointing topLeft
    vec3 bottomRight = vec3(1, -1, 0);   // Pointing bottomRight
    vec3 bottomLeft = vec3(-1, -1, 0);   // Pointing bottomLeft
    vec3 top = vec3(0, 1, 1);            // Pointing top
    vec3 bottom = vec3(0, 1, -1);        // Pointing bottom
    vec3 topRightBack = vec3(1, 1, 1);   // Pointing topRightBack
    vec3 topRightFront = vec3(1, 1, -1); // Pointing topRightFront
    vec3 topRightUp = vec3(1, 1, 1);     // Pointing topRightUp
    vec3 topRightDown = vec3(1, 1, -1);  // Pointing topRightDown

    vector<vec3> allDirections = {topRight, topLeft, bottomRight, bottomLeft, top, bottom, topRightBack, topRightFront, topRightUp, topRightDown};

    for (size_t i = 0; i < allDirections.size(); i++)
    {
        camera cam(size, size, step, camOrigin, camXDirection, camYDirection, allDirections[i]);
        std::cout << "_________Face Coloring_______________" << std::endl;
        double scaling = size * step / 2;
        point offset = point(0, size * step / 2, size * step / 2);
        object obj(primitive::cube, scaling, offset + point(0, 0, 0));
        std::cout << "________________________" << std::endl;
        // Create a space and assign the object
        space s({obj});

        // Add the camera to the space
        s.cameras.push_back(cam);

        // Trigger the camera rays
        s.triggerCameraRay();
        ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Room" + std::to_string(i) + ".ppm");
    }
}
// Generate a video of the object as the camera change direction
void generateVideo()
{
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size = 500;
    double step = 0.002;

    point camOrigin(0, 0, 0);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(0, 0, 0);  // Pointing downward

    size_t frame = 100;
    double frameStep = .1;

    double scaling = size * step / 2;
    point offset = point(0, size * step / 2, size * step / 2);
    object obj(primitive::cone, scaling, offset + point(0, 0, 0));
    space s({obj});

    vec3 tempDirection = vec3(0, 0, 0);
    for (size_t i = 1; i <= frame; i++)
    {
        double x = (tempDirection.x() + frameStep) / (double)1.4;
        double y = (tempDirection.y() + frameStep) / (double)1.1;
        double z = (tempDirection.z() + frameStep) / (double)1.5;

        tempDirection = (vec3(x, y, z));
        if (tempDirection.x() >= 1)
        {
            tempDirection = (vec3(-1, tempDirection.y(), tempDirection.z()));
        }
        if (tempDirection.z() >= 1)
        {
            tempDirection = (vec3(tempDirection.x(), tempDirection.y(), -1));
        }
        if (tempDirection.y() >= 1)
        {
            tempDirection = (vec3(tempDirection.x(), -1, tempDirection.z()));
        }
        // cout << "tempDirection: " << tempDirection << endl;

        // Add the camera to the space
        s.cameras.clear();
        camera cam(size, size, step, camOrigin, camXDirection, camYDirection, tempDirection);
        s.cameras.push_back(cam);

        // Trigger the camera rays
        s.triggerCameraRay();
        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFilePPM(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
    }
    // Convert PPM to Video
    std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}
// Generate a video of the object as the camera change direction
void generateVideo1()
{

    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.01;
    unsigned int height = size;
    unsigned int width = size;

    // Camera grid dimensions
    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / num_cameraX;
    unsigned int camera_width = width / num_cameraY;

    // Initialize cameras grid
    vector<vector<camera>> camerasGrid(num_cameraX, vector<camera>(num_cameraY));

    point camOrigin(-3, -3, -3);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(1, 1, 1);  // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;

            camerasGrid[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
        }
    }

    space s = space();

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty())
    {
        std::cout << "No cameras " << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;
    size_t frame = 1440 / 2;

    double startSize = .2;
    double endSize = 3;
    double changepSize = .01;
    double currentSize = startSize;
    int direction = 1;

    for (size_t i = 1; i <= frame; i++)
    {
        currentSize += changepSize * direction;

        if (currentSize >= endSize)
        {
            direction = -1;
        }
        else if (currentSize <= startSize)
        {
            direction = 1;
        }

        // Create space and add object
        s.cameras.at(0).clear();
        s.cameras.at(1).clear();
        s.cameras.at(2).clear();
        s.cameras.at(3).clear();
        s.obj.clear();
        object obj(primitive::cube, currentSize, point(-2, 0, 0));
        s.obj.push_back(obj);

        std::vector<std::future<void>> futures;
        s.threadedCameraRay(futures);

        // Wait for all threads to complete
        for (auto &future : futures)
        {
            future.get();
        }

        std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

        std::cout << "_________Stitching_______________" << std::endl;

        // Stitch images into a single output
        std::cout << "Combined Width: " << combinedWidth << std::endl;
        std::cout << "Combined Height: " << combinedHeight << std::endl;

        vector<vector<image>> images; // Placeholder for stitched images

        for (int i = static_cast<int>(num_cameraX) - 1; i >= 0; i--)
        { // Outer loop iterates backward
            vector<image> rowImages;
            for (int j = static_cast<int>(num_cameraY) - 1; j >= 0; j--)
            { // Outer loop iterates backward
                rowImages.push_back(s.cameras[i * num_cameraY + j].getimage());
            }
            images.push_back(rowImages);
        }

        // swap the first and last image
        image tempimg = images[0][0];
        images[0][0] = images[num_cameraX - 1][num_cameraY - 1];
        images[num_cameraX - 1][num_cameraY - 1] = tempimg;

        image stitchedImage(combinedWidth, combinedHeight, images);
        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFilePPM(stitchedImage, "Output/Step" + std::to_string(i) + ".ppm");
    }
    // Convert PPM to Video
    // std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";
    // for 1440 frames its better to use this :
    std::string convertCommand = "ffmpeg -framerate 60 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}
// Generate a video of the object as the camera change direction
void generateVideo2()
{
    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.01;
    unsigned int height = size;
    unsigned int width = size;

    // Camera grid dimensions
    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / num_cameraX;
    unsigned int camera_width = width / num_cameraY;

    // Initialize cameras grid
    vector<vector<camera>> camerasGrid(num_cameraX, vector<camera>(num_cameraY));

    point camOrigin(-3, -3, -3);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(1, 1, 1);  // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;

            camerasGrid[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
        }
    }

    space s = space();

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty())
    {
        std::cout << "No cameras " << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;
    size_t frame = 1440 / 5;

    double startSize = .2;
    double endSize = 3;
    double changepSize = .01;
    double currentSize = startSize;
    double currentSize1 = startSize;
    int direction = 1;
    int direction1 = -1;

    for (size_t i = 1; i <= frame; i++)
    {
        currentSize += changepSize * direction;

        if (currentSize >= endSize)
        {
            direction = -1;
        }
        else if (currentSize <= startSize)
        {
            direction = 1;
        }

        currentSize1 += changepSize * direction1;

        if (currentSize1 >= endSize)
        {
            direction1 = -1;
        }
        else if (currentSize1 <= startSize)
        {
            direction1 = 1;
        }

        // Create space and add object
        s.cameras.at(0).clear();
        s.cameras.at(1).clear();
        s.cameras.at(2).clear();
        s.cameras.at(3).clear();
        s.obj.clear();
        object obj(primitive::cube, currentSize, point(-2, 0, 0));
        object obj1(primitive::cone, currentSize1, point(-1.5, 0, 0));
        s.obj.push_back(obj);
        s.obj.push_back(obj1);

        std::vector<std::future<void>> futures;
        s.threadedCameraRay(futures);

        // Wait for all threads to complete
        for (auto &future : futures)
        {
            future.get();
        }

        std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

        std::cout << "_________Stitching_______________" << std::endl;

        // Stitch images into a single output
        std::cout << "Combined Width: " << combinedWidth << std::endl;
        std::cout << "Combined Height: " << combinedHeight << std::endl;

        vector<vector<image>> images; // Placeholder for stitched images

        for (int i = static_cast<int>(num_cameraX) - 1; i >= 0; i--)
        { // Outer loop iterates backward
            vector<image> rowImages;
            for (int j = static_cast<int>(num_cameraY) - 1; j >= 0; j--)
            { // Outer loop iterates backward
                rowImages.push_back(s.cameras[i * num_cameraY + j].getimage());
            }
            images.push_back(rowImages);
        }

        // swap the first and last image
        image tempimg = images[0][0];
        images[0][0] = images[num_cameraX - 1][num_cameraY - 1];
        images[num_cameraX - 1][num_cameraY - 1] = tempimg;

        image stitchedImage(combinedWidth, combinedHeight, images);
        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFilePPM(stitchedImage, "Output/Step" + std::to_string(i) + ".ppm");
    }
    // Convert PPM to Video
    // std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";
    // for 1440 frames its better to use this :
    std::string convertCommand = "ffmpeg -framerate 12 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}
// testing the split rays function with threads on a cube with right resolution and a different camera orientation
void split_raysThreadsCube()
{
    // Define the grid size and step
    unsigned int scaleMultiplier = 4;
    unsigned int size = 400 * scaleMultiplier;
    double step = 0.01 / scaleMultiplier;
    unsigned int height = size;
    unsigned int width = size;

    // Camera grid dimensions
    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / num_cameraX;
    unsigned int camera_width = width / num_cameraY;

    // Initialize cameras grid
    vector<vector<camera>> camerasGrid(num_cameraX, vector<camera>(num_cameraY));

    point camOrigin(-3, -3, -3);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(1, 1, 1);  // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection * shiftX + camYDirection * shiftY) / 2;

            camerasGrid[i][j] = camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
        }
    }

    // Create space and add object
    space s({object(primitive::cube, 1, point(-2, 0, 0))});

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty() || s.obj.empty())
    {
        std::cout << "No cameras or objects to process." << std::endl;
        return;
    }

    std::vector<std::future<void>> futures;
    s.threadedCameraRay(futures);

    // Wait for all threads to complete
    for (auto &future : futures)
    {
        future.get();
    }

    std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

    std::cout << "_________Stitching_______________" << std::endl;

    // Stitch images into a single output
    std::cout << "Combined Width: " << combinedWidth << std::endl;
    std::cout << "Combined Height: " << combinedHeight << std::endl;

    vector<vector<image>> images; // Placeholder for stitched images

    for (int i = static_cast<int>(num_cameraX) - 1; i >= 0; i--)
    { // Outer loop iterates backward
        vector<image> rowImages;
        for (int j = static_cast<int>(num_cameraY) - 1; j >= 0; j--)
        { // Outer loop iterates backward
            rowImages.push_back(s.cameras[i * num_cameraY + j].getimage());
        }
        images.push_back(rowImages);
    }

    // swap the first and last image
    image tempimg = images[0][0];
    images[0][0] = images[num_cameraX - 1][num_cameraY - 1];
    images[num_cameraX - 1][num_cameraY - 1] = tempimg;

    image stitchedImage(combinedWidth, combinedHeight, images);
    ImageRenderer::renderToFile(stitchedImage, "stitched.ppm");
}
// testing the ability to make a camera have a perspective like a field of view
void testPerspective()
{
    std::cout << "_________Space Test_______________" << std::endl;
    // Define the grid size
    float resolutionMultiplier = 1;
    unsigned int size = 500 * resolutionMultiplier;
    double step = 0.01 / resolutionMultiplier;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    // this is a vector of vectors of tuple of ray and pointer to the ray that point to the grad ray above
    std::vector<std::vector<ray>> gridRay1;
    float perspectiveScale = 100;
    float perspectiveForce = 100;
    vec3 direction(0, 1, 0);

    // create a grid of rays that will be used to create the camera
    // and another grid of rays that will be used to create a difference in space between each ray
    // the  y axis in this case is depth
    // the z is left and right
    // and the x is height
    vec3 origineOffset(1, 1, 1.3);
    origineOffset *= 3;
    for (unsigned i = 0; i < size; ++i)
    {
        std::vector<ray> row;
        std::vector<ray> row1;
        for (unsigned j = 0; j < size; ++j)
        {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, 0, j * step);
            origin += origineOffset;
            point origin1(i * step * perspectiveScale, 0.5, j * step * perspectiveScale);
            origin1 += origineOffset;
            ray tempRy = ray(origin, direction);
            ray tempRy1 = ray(origin1, direction);
            row.push_back(tempRy);
            row1.push_back(tempRy1);
        }
        gridRay.push_back(row);
        gridRay1.push_back(row1);
    }
    // and we get the center point of the second grid
    vec3 MovedVectorBetweenOldPositionAndNew = gridRay.at(size / 2).at(size / 2).get(perspectiveForce) - gridRay1.at(size / 2).at(size / 2).getOrigine();

    // foreach loop that will go threw grid ray and add the moved vector to the direction of the ray
    for (size_t i = 0; i < gridRay1.size(); i++)
    {
        for (size_t j = 0; j < gridRay1.at(i).size(); j++)
        {
            point current = gridRay1.at(i).at(j).getOrigine();
            gridRay1.at(i).at(j).setOrigine(current + MovedVectorBetweenOldPositionAndNew);
        }
    }

    for (size_t i = 0; i < gridRay.size(); i++)
    {
        for (size_t j = 0; j < gridRay.at(i).size(); j++)
        {
            vec3 newDirection = gridRay1.at(i).at(j).getOrigine() - gridRay.at(i).at(j).getDirection();
            newDirection = gmath::normalize(newDirection);
            gridRay.at(i).at(j).setDirection(newDirection);
        }
    }
    gridRay1.clear();

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling = 10;
    point offset = point(0, -3, 1.5);
    object obj(primitive::cube, scaling, offset);

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
    s.saveImages();

    std::cout << "________________________" << std::endl;

    // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
    string folderName = "TestRender";
    std::filesystem::create_directories(folderName);
    std::string filename = folderName + "/CubeRenderPerspectiveScale" + std::to_string((int)perspectiveScale);
    std::replace(filename.begin(), filename.end(), '.', '_'); // Replace '.' with '_'

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), filename + ".ppm");
}
// create a video of a cube in differend perspective like a field of view
void testPerspectiveLoop()
{
    for (float vv = 0; vv < 20; vv += .1)
    {

        std::cout << "_________Space Test_______________" << std::endl;
        // Define the grid size
        float resolutionMultiplier = 1;
        unsigned int size = 500 * resolutionMultiplier;
        double step = 0.01 / resolutionMultiplier;

        // Create a vector of rays pointing toward the plane z = 0
        std::vector<std::vector<ray>> gridRay;
        // this is a vector of vectors of tuple of ray and pointer to the ray that point to the grad ray above
        std::vector<std::vector<ray>> gridRay1;
        float perspectiveScale = 2;
        perspectiveScale = vv;
        vec3 direction(0.5, 0.5, -3);

        // create a grid of rays that will be used to create the camera
        // and another grid of rays that will be used to create a difference in space between each ray
        for (unsigned i = 0; i < size; ++i)
        {
            std::vector<ray> row;
            std::vector<ray> row1;
            for (unsigned j = 0; j < size; ++j)
            {
                // Rays originate from above (z = 1) and point downward toward z = 0
                point origin(i * step, j * step, 6);
                point origin1(i * step * perspectiveScale, j * step * perspectiveScale, 6 * perspectiveScale);
                ray tempRy = ray(origin, direction);
                ray tempRy1 = ray(origin1, direction + vec3(0, 0, -1));
                row.push_back(tempRy);
                row1.push_back(tempRy1);
            }
            gridRay.push_back(row);
            gridRay1.push_back(row1);
        }
        // and we get the center point of the second grid
        vec3 MovedVectorBetweenOldPositionAndNew = gridRay.at(size / 2).at(size / 2).get(10) - gridRay1.at(size / 2).at(size / 2).getOrigine();
        // cout << MovedVectorBetweenOldPositionAndNew << endl;

        // foreach loop that will go threw grid ray and add the moved vector to the direction of the ray
        for (size_t i = 0; i < gridRay1.size(); i++)
        {
            for (size_t j = 0; j < gridRay1.at(i).size(); j++)
            {
                point current = gridRay1.at(i).at(j).getOrigine();
                gridRay1.at(i).at(j).setOrigine(current + MovedVectorBetweenOldPositionAndNew);
            }
        }

        for (size_t i = 0; i < gridRay.size(); i++)
        {
            for (size_t j = 0; j < gridRay.at(i).size(); j++)
            {
                vec3 newDirection = gridRay1.at(i).at(j).getOrigine() - gridRay.at(i).at(j).getDirection();
                newDirection = gmath::normalize(newDirection);
                gridRay.at(i).at(j).setDirection(newDirection);
            }
        }
        gridRay1.clear();

        // Create a camera with the grid of rays
        camera cam(size, size, gridRay);

        double scaling = 3;
        point offset = point(2, 1, 1);

        // Bottom face
        object obj(primitive::cube, scaling, offset);

        // Create a space and assign the object
        space s({obj});

        // Add the camera to the space
        s.cameras.push_back(cam);

        // Trigger the camera rays
        s.triggerCameraRay();
        s.saveImages();

        std::cout << "________________________" << std::endl;

        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        string folderName = "TestRender";
        std::filesystem::create_directories(folderName);
        std::string filename = folderName + "/CubeRenderPerspectiveScale" + std::to_string(perspectiveScale);
        std::replace(filename.begin(), filename.end(), '.', '_'); // Replace '.' with '_'

        ImageRenderer::renderToFile(s.cameras.at(0).getimage(), filename + ".ppm");
    }
}
// also create a video of a cube in differend perspective like a field of view
void testPerspectiveLoop1()
{
    int increment = 0;
    for (float vv = 0; vv < 2; vv += .05)
    {
        std::cout << "_________Space Test_______________" << std::endl;
        // Define the grid size
        float resolutionMultiplier = 1;
        unsigned int size = 500 * resolutionMultiplier;
        double step = 0.01 / resolutionMultiplier;

        // Create a vector of rays pointing toward the plane z = 0
        std::vector<std::vector<ray>> gridRay;
        // this is a vector of vectors of tuple of ray and pointer to the ray that point to the grad ray above
        std::vector<std::vector<ray>> gridRay1;
        float perspectiveScale = 100;
        float perspectiveForce = 100;
        vec3 direction(0, 1, 0);

        // create a grid of rays that will be used to create the camera
        // and another grid of rays that will be used to create a difference in space between each ray
        // the  y axis in this case is depth
        // the z is left and right
        // and the x is height
        vec3 origineOffset(1, -2 + vv * 2, 1.3);
        origineOffset *= 3;
        for (unsigned i = 0; i < size; ++i)
        {
            std::vector<ray> row;
            std::vector<ray> row1;
            for (unsigned j = 0; j < size; ++j)
            {
                // Rays originate from above (z = 1) and point downward toward z = 0
                point origin(i * step, 0, j * step);
                origin += origineOffset;
                point origin1(i * step * perspectiveScale, 0.5, j * step * perspectiveScale);
                origin1 += origineOffset;
                ray tempRy = ray(origin, direction);
                ray tempRy1 = ray(origin1, direction);
                row.push_back(tempRy);
                row1.push_back(tempRy1);
            }
            gridRay.push_back(row);
            gridRay1.push_back(row1);
        }
        // and we get the center point of the second grid
        vec3 MovedVectorBetweenOldPositionAndNew = gridRay.at(size / 2).at(size / 2).get(perspectiveForce) - gridRay1.at(size / 2).at(size / 2).getOrigine();

        // foreach loop that will go threw grid ray and add the moved vector to the direction of the ray
        for (size_t i = 0; i < gridRay1.size(); i++)
        {
            for (size_t j = 0; j < gridRay1.at(i).size(); j++)
            {
                point current = gridRay1.at(i).at(j).getOrigine();
                gridRay1.at(i).at(j).setOrigine(current + MovedVectorBetweenOldPositionAndNew);
            }
        }

        for (size_t i = 0; i < gridRay.size(); i++)
        {
            for (size_t j = 0; j < gridRay.at(i).size(); j++)
            {
                vec3 newDirection = gridRay1.at(i).at(j).getOrigine() - gridRay.at(i).at(j).getDirection();
                newDirection = gmath::normalize(newDirection);
                gridRay.at(i).at(j).setDirection(newDirection);
            }
        }
        gridRay1.clear();

        // Create a camera with the grid of rays
        camera cam(size, size, gridRay);

        double scaling = 10;
        point offset = point(0, -3, 1.5);
        object obj(primitive::cube, scaling, offset);

        // Create a space and assign the object
        space s({obj});

        // Add the camera to the space
        s.cameras.push_back(cam);

        // Trigger the camera rays
        s.triggerCameraRay();
        // s.saveImages();

        std::cout << "________________________" << std::endl;
        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        string folderName = "NewRenderForVideo";
        std::filesystem::create_directories(folderName);
        std::string filename = folderName + "/YshiftCubeTest" + std::to_string(perspectiveScale);
        std::replace(filename.begin(), filename.end(), '.', '_'); // Replace '.' with '_'

        ImageRenderer::renderToFilePPM(s.cameras.at(0).getimage(), filename + std::to_string(increment) + ".ppm");
        increment++;
    }
    // Convert PPM to Video
    // std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";
    // for 1440 frames its better to use this :
    std::string convertCommand = "ffmpeg -framerate 12 -i NewRenderForVideo/YshiftCubeTest%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}

vector<vector<camera>> splitCameraFunc(camera cam, size_t split)
{
    size_t height = cam.getheight();
    size_t width = cam.getwidth();

    // cout << height / split << endl;

    // cout << width / split << endl;

    vector<vector<ray>> gridRay = cam.getGridRay();
    vector<vector<camera>> SplitCamera;

    for (size_t i = 0; i < split; i++)
    {
        vector<camera> tempsplit;
        for (size_t j = 0; j < split; j++)
        {
            camera tempcam = camera();
            tempsplit.push_back(tempcam);
        }
        SplitCamera.push_back(tempsplit);
    }

    size_t split_height = height / split;
    size_t split_width = width / split;

    for (size_t k = 0; k < split; k++) // to affect the offset on the x
    {
        for (size_t z = 0; z < split; z++)
        {
            /* code */
            size_t x = k * split_height;
            size_t y = z * split_width;
            vector<vector<ray>> gridRaySplit;

            for (size_t i = 0; i < split_height; i++) // this loop will explore a 2d vectore based on different offsets this will be the x effect
            {
                vector<ray> tempgridRaySplit;
                for (size_t j = 0; j < split_width; j++) // this will be the y effect
                {
                    // gridRay.at(i + x).at(j + y);
                    //gridRay[i + x][j + y];
                    // cout << "--(x:" << i + x << " || y:" << j + y << ")";
                    tempgridRaySplit.push_back(gridRay.at(i + x).at(j + y));
                }

                gridRaySplit.push_back(tempgridRaySplit);
                /*
                cout
                    << endl;
                cout << endl;

                */
            }

            camera newSplitCamera = camera(split_width, split_height, gridRaySplit);
            SplitCamera.at(k).at(z) = newSplitCamera;
            // cerr  << "chunk" << endl;
            // cout << endl;
        }

        /* cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cerr << "split" << endl;*/
    }

    for (size_t i = 0; i < split; i++)
    {
        for (size_t j = 0; j < split; j++)
        {
            // cout << SplitCamera.at(i).at(j) << endl;
        }
    }

    return SplitCamera;
}

void splitCamera()
{
    // Define the grid size and step
    unsigned int size = 4;
    double step = 1;

    point camOrigin(0, 0, 0);
    vec3 camYDirection(0, 0, 1); // Pointing upward
    vec3 camXDirection(0, 1, 0); // Pointing right
    vec3 rayDirection(1, 1, -1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, vec3(0, 0, 1));

    // cout << cam1 << endl;
    size_t split = 4;

    vector<vector<camera>> SplitCamera = splitCameraFunc(cam1, split);

    for (size_t i = 0; i < split; i++)
    {
        for (size_t j = 0; j < split; j++)
        {
            // cout << SplitCamera.at(i).at(j) << endl;
        }
        cout << "_________________" << endl;
        cout << endl;
    }
}

void splitCamera1()
{
    // Define the grid size and step
    unsigned int size = 400;
    double step = 0.1;

    point camOrigin(10, 10, 10);
    vec3 camYDirection(0, 0, -1); // Pointing upward
    vec3 camXDirection(0, 1, 0);  // Pointing right
    vec3 rayDirection(1, 1, -1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, vec3(0, 0, 1));

    size_t split = 2;

    vector<vector<camera>> SplitCamera = splitCameraFunc(cam1, split);

    std::cout << "_________Face Coloring_______________" << std::endl;
    double scaling = size * step / 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(0, 0, 0));
    std::cout << "________________________" << std::endl;
    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    for (size_t i = 0; i < split; i++)
    {
        for (size_t j = 0; j < split; j++)
        {
            s.cameras.push_back(SplitCamera.at(i).at(j));
        }
    }

    // Trigger the camera rays
    s.triggerCameraRay();
    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Room" + std::to_string(0) + ".ppm");
    ImageRenderer::renderToFile(s.cameras.at(1).getimage(), "Room" + std::to_string(1) + ".ppm");
    ImageRenderer::renderToFile(s.cameras.at(2).getimage(), "Room" + std::to_string(2) + ".ppm");
    ImageRenderer::renderToFile(s.cameras.at(3).getimage(), "Room" + std::to_string(3) + ".ppm");
}

void splitCameraThreadingV2()
{
    // Define the grid size and step
    size_t ratio = 40;
    unsigned int size = 400 / ratio;
    double step = .01f * ratio;

    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);  // Pointing upward
    vec3 camXDirection(0, -1, 1); // Pointing right

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    // cout << cam1 << endl;
    size_t split = 2;

    vector<vector<camera>> SplitCamera = splitCameraFunc(cam1, split);

    std::cout << "_________Face Coloring_______________" << std::endl;
    double scaling = 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2));
    std::cout << "________________________" << std::endl;
    // Create a space and assign the object
    space s({obj});

    for (size_t i = 0; i < split; i++) // push all the cameras into the camera vector of the space i want to render
    {
        for (size_t j = 0; j < split; j++)
        {
            s.cameras.push_back(SplitCamera.at(i).at(j));
        }
    }

    std::vector<std::future<void>> futures;
    s.threadedCameraRay(futures);

    // Wait for all threads to complete
    for (auto &future : futures)
    {
        future.get();
    }

    size_t increment = 0;

    //  s.saveImages();

    vector<vector<image>> images; // Placeholder for stitched images

    increment = 0;
    for (size_t i = 0; i < SplitCamera.size(); i++)
    { // Outer loop iterates backward
        vector<image> rowImages;
        for (size_t j = 0; j < SplitCamera.at(i).size(); j++)
        { // Outer loop iterates backward
            rowImages.push_back(s.cameras[increment].getimage());
            increment++;
        }
        images.push_back(rowImages);
    }

    image stitchedImage(size, size, images);
    ImageRenderer::renderToFile(stitchedImage, "stitched.ppm");
}

void testraytracing()
{
    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 400 / ratio;
    double step = .01f * ratio;

    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    std::cout
        << "_________Face Coloring_______________" << std::endl;
    double scaling = 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2));

    object obj1(primitive::cube, 2, offset + point(1.5, 1, 1));
    obj1.isEmisive = true;

    std::cout
        << "________________________" << std::endl;

    // Create a space and assign the object
    space s({obj, obj1});
    s.cameras.push_back(cam1);
    s.triggerRayTrace(2);

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "XXXX.ppm");
}

void testraytracing2()
{
    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 400 / ratio;
    double step = .01f * ratio;

    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    std::cout
        << "_________Face Coloring_______________" << std::endl;
    double scaling = 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2));

    object obj1(primitive::cube, 2, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(0, 0, -1));
    obj1.isEmisive = true;

    std::cout
        << "________________________" << std::endl;

    // Create a space and assign the object
    space s({obj, obj1});
    s.cameras.push_back(cam1);
    s.triggerRayTrace(2);

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "XXXX.ppm");
}

void testOptimizedrender()
{
    time_t timestamp = time(NULL);

    // cout << "Current time: " << asctime(&datetime);

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 4000 / ratio;
    double step = .01f * ratio;

    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    std::cout
        << "_________Face Coloring_______________" << std::endl;
    double scaling = 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(0, 0, 0));

    object obj1(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(0, 0, 8));

    object obj2(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(2, 2, 0));

    object obj3(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(2, 2, 8));

    object obj4(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(4, 0, 0));

    object obj5(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(4, 0, 8));

    vector<object> test;
    double offsetmultiplier = 5;
    for (size_t i = 0; i < 11; i++)
    {
        for (size_t j = 0; j < 11; j++)
        {
            object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            test.push_back(obj);
        }
    }

    std::cout
        << "________________________" << std::endl;

    // Create a space and assign the object
    space s(test);
    s.cameras.push_back(cam1);
    s.triggerCameraRayOptimized();

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "XXXX.ppm");

    time_t timestamp1 = time(NULL);

    // cout << "Time after optimized render: " << asctime(&datetime1);

    double time_difference = difftime(timestamp1, timestamp);

    cout << "Time difference between events: " << time_difference << " seconds" << endl;
}

void testNonOptimizedrender()
{
    time_t timestamp = time(NULL);

    // cout << "Current time: " << asctime(&datetime);

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 4000 / ratio;
    double step = .01f * ratio;

    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    std::cout
        << "_________Face Coloring_______________" << std::endl;
    double scaling = 2;
    point offset = point(0, 0, 0);
    object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(0, 0, 0));

    object obj1(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(0, 0, 8));

    object obj2(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(2, 2, 0));

    object obj3(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(2, 2, 8));

    object obj4(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(4, 0, 0));

    object obj5(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(4, 0, 8));

    vector<object> test;
    double offsetmultiplier = 5;
    for (size_t i = 0; i < 11; i++)
    {
        for (size_t j = 0; j < 11; j++)
        {
            object obj(primitive::cube, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            test.push_back(obj);
        }
    }

    std::cout
        << "________________________" << std::endl;

    // Create a space and assign the object
    space s(test);
    s.cameras.push_back(cam1);
    s.triggerCameraRay();

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "XXXX.ppm");

    time_t timestamp1 = time(NULL);

    // cout << "Time after Nonoptimized render: " << asctime(&datetime1);

    double time_difference = difftime(timestamp1, timestamp);

    cout << "Time difference between events: " << time_difference << " seconds" << endl;
}

void testQuaternion()
{
    // Define a position vector and a rotation axis
    vec3 position(0, 2, 0);
    vec3 axis(1, 0, 0);

    double angle = 100;
    angle = angle * 3.14159 / 180; // Convert angle to radians

    // Create a quaternion from the axis-angle representation
    quaternion rotationQ = quaternion::fromAxisAngle(axis, angle);

    // Compute the inverse rotation (quaternion conjugate for unit quaternions)
    quaternion inverseRotationQ = rotationQ.conjugate();

    // Convert position vector to a pure quaternion
    quaternion positionQ(0, position.x(), position.y(), position.z());

    // Apply rotation using quaternion multiplication
    quaternion rotatedQ = rotationQ * positionQ * inverseRotationQ;

    // Extract the rotated position
    vec3 rotatedPosition = vec3(rotatedQ.x(), rotatedQ.y(), rotatedQ.z());

    // Print the results
    std::cout << "Original Position: " << position << std::endl;
    std::cout << "Rotated Position: " << rotatedPosition << std::endl;
}

void testRotationOnMesh()
{
    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 600 / ratio;
    double step = .01f * ratio;

    point camOrigin = point(-9, 3, -10) / 3;
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    std::cout
        << "_________Face Coloring_______________" << std::endl;
    double scaling = 1.8;
    point offset = point(0, 0, 0);
    vec3 axis(0, 0, 1);
    double angle = 90;
    object obj(primitive::suzane, scaling, offset, angle, axis);
    // cout << obj << endl;

    std::cout
        << "________________________" << std::endl;

    // Create a space and assign the object
    space s({obj});
    s.cameras.push_back(cam1);
    s.triggerCameraRay();

    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "YYYY.ppm");
}

// Generate a video of the object as the camera change direction
void testRotationOnMeshVideo()
{

    std::cout << "_________Space Test_______________" << std::endl;
    size_t frame = 1440;

    // setup for video
    double endAngle = 360;
    double videoStep = endAngle / frame;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 1000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(0, 0, -3);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 4;
    point offset = point(0, 0, 0);
    vec3 axis(0, 1, 0);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    double angle = 0;
    for (size_t i = 1; i <= frame; i++)
    {
        angle += videoStep;

        object obj(primitive::cube, scaling, offset + point(scaling, scaling, scaling) + point(-2.5, -6, 0), angle, axis);

        // Create a space and assign the object
        space s({obj});
        s.cameras.push_back(cam1);
        s.triggerCameraRay();

        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFilePPM(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
    }
    // Convert PPM to Video
    // std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";
    // for 1440 frames its better to use this :
    std::string convertCommand = "ffmpeg -framerate 60 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}

// Generate a video of the object as the camera change direction
void testRotationOnMeshVideo1()
{

    std::cout << "_________Space Test_______________" << std::endl;
    size_t frame = 1440;

    // setup for video
    double endAngle = 360;
    double videoStep = endAngle / frame;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 8;
    point offset = point(0, 0, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);

    double angle = 0;
    for (size_t i = 1; i <= frame; i++)
    {
        angle += videoStep;
        object obj(primitive::cube, scaling, offset, angle, axis);

        // Create a space and assign the object
        space s({obj});
        s.cameras.push_back(cam1);
        s.triggerCameraRay();

        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFilePPM(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
    }
    // Convert PPM to Video
    // std::string convertCommand = "ffmpeg -framerate 2 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";
    // for 1440 frames its better to use this :
    std::string convertCommand = "ffmpeg -framerate 60 -i Output/Step%d.ppm -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p output_video.mp4";

    // std::cout << "convertCommand: " << convertCommand << std::endl;
    int convertResult = system(convertCommand.c_str());
    if (convertResult != 0)
    {
        throw std::runtime_error("Error: Failed to convert PPM to Video.");
        return;
    }
}

void testPerlin()
{

    perlin p(2,2);
    cout <<p;
    p.buildPerlin();
    p.Save();


}


void testGraph()
{
    size_t x=5,y=5;

    vector <vector<graphNode>> gridNode;
    int increment=0;
    for (size_t i = 0 ; i < x ; i++)
    {
        vector<graphNode> temprow;
        for (size_t z = 0 ; z < y ; z++)
        {
            graphNode tempNode;
            tempNode.value=increment;
            temprow.push_back(tempNode);
        }   
        increment++;
        gridNode.push_back(temprow);
    }


     for (size_t i = 0 ; i < gridNode.size() ; i++)
    {
        for (size_t z = 0 ; z < gridNode.at(i).size() ; z++)
        {
            if(graph::graphConstraing(i+1,z,y,x))
            {
                gridNode.at(i).at(z).children.push_back(&gridNode.at(i+1).at(z));
            }
             if(graph::graphConstraing(i-1,z,y,x))
            {
                gridNode.at(i).at(z).children.push_back(&gridNode.at(i-1).at(z));
            }
             if(graph::graphConstraing(i,z+1,y,x))
            {
                gridNode.at(i).at(z).children.push_back(&gridNode.at(i).at(z+1));
            }
             if(graph::graphConstraing(i,z-1,y,x))
            {
                gridNode.at(i).at(z).children.push_back(&gridNode.at(i).at(z-1));
            }
        }   
    }


    graphNode* current = &(gridNode.at(0).at(0));

    for (size_t i = 0; i < 25; i++) {
        cout << current->value << endl;  // use -> for pointer access

        if (!current->children.empty()) {
            current = current->children.at(0);  // move to first child
        } else {
            cout << "no childrens found";
            break;
        }
    }


   

    
}

void testGraph1()
{

    // the entended square grid
    size_t x=10,y=10;

    // creating the graph like structure
    vector<vector<tuple<graphNode, object>>> gridNode;
    int increment=0;
    for (size_t i = 0 ; i < x ; i++)
    {
        vector<tuple<graphNode, object>> temprow;
        for (size_t z = 0 ; z < y ; z++)
        {
            graphNode tempNode;
            tempNode.value=increment;
            tempNode.index={i,z};
            temprow.push_back(std::make_tuple(tempNode, object{}));
        }   
        increment++;
        gridNode.push_back(temprow);
    }


     for (size_t i = 0 ; i < gridNode.size() ; i++)
    {
        for (size_t z = 0 ; z < gridNode.at(i).size() ; z++)
        {
            if(graph::graphConstraing(i+1,z,y,x))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i+1).at(z)));
            }
             if(graph::graphConstraing(i-1,z,y,x))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i-1).at(z)));
            }
             if(graph::graphConstraing(i,z+1,y,x))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i).at(z+1)));
            }
             if(graph::graphConstraing(i,z-1,y,x))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i).at(z-1)));
            }
        }   
    }


   

   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);

    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =10;
    for (size_t i = 0; i < grid_sizer; i++)
    {
        for (size_t j = 0; j < grid_sizer; j++)
        {
            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(gridNode[i][j]) = obj;  
        }
    }

    
     graphNode* current = &(get<0>(gridNode.at(0).at(0)));
     std::vector<graphNode*> nextNodes;
     nextNodes.push_back(current);

     // Create a space and assign the object

    for (size_t i = 0; i < 5; i++) {
        cout << current->value << endl;  // use -> for pointer access
        
        if (!current->children.empty()) {
            for (const auto& child : current->children) {
                nextNodes.push_back(child);

            }

            int the_x=current->index[0];
            int the_y=current->index[0];
            get<1>(gridNode[the_x][the_y]).setColor(color(0,0,255));
           std::vector<object> allObjects;
            for (const auto& row : gridNode) {
                for (const auto& cell : row) {
                    allObjects.push_back(std::get<1>(cell));
                }
            }

            space s(allObjects);
            s.cameras.push_back(cam1);
            s.triggerCameraRayOptimized();
            // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
            std::filesystem::create_directories("Output");
            ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
            


            current->explored=true;
            current = current->children.at(0);  // move to first child
        } else {
            cout << "no childrens found";
            break;
        }
    }

}



void testGraph2()
{
    // the entended square grid
    size_t x=10,y=10;
    graph main_graph(x,y);
   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);

    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =10;
    for (size_t i = 0; i < grid_sizer; i++)
    {
        for (size_t j = 0; j < grid_sizer; j++)
        {
            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(main_graph.gridNode[i][j]) = obj;  
        }
    }

    
     graphNode* current = &(get<0>(main_graph.gridNode.at(0).at(0)));
     std::vector<graphNode*> nextNodes;
     nextNodes.push_back(current);

     // Create a space and assign the object

    for (size_t i = 0; i < 5; i++) {
        cout << current->value << endl;  // use -> for pointer access
        
        if (!current->children.empty()) {
            for (const auto& child : current->children) {
                nextNodes.push_back(child);

            }

            int the_x=current->index[0];
            int the_y=current->index[0];
            get<1>(main_graph.gridNode[the_x][the_y]).setColor(color(0,0,255));
           std::vector<object> allObjects;
            for (const auto& row : main_graph.gridNode) {
                for (const auto& cell : row) {
                    allObjects.push_back(std::get<1>(cell));
                }
            }

            space s(allObjects);
            s.cameras.push_back(cam1);
            s.triggerCameraRayOptimized();
            // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
            std::filesystem::create_directories("Output");
            ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
            


            current->explored=true;
            current = current->children.at(0);  // move to first child
        } else {
            cout << "no childrens found";
            break;
        }
    }

}


void testGraph3()
{
    // the entended square grid
    size_t x=10,y=10;
    graph main_graph(x,y);
   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);

    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =10;
    for (size_t i = 0; i < grid_sizer; i++)
    {
        for (size_t j = 0; j < grid_sizer; j++)
        {
            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(main_graph.gridNode[i][j]) = obj;  
        }
    }

    
     graphNode* current = &(get<0>(main_graph.gridNode.at(0).at(0)));
     std::vector<graphNode*> nextNodes;
     nextNodes.push_back(current);

     // Create a space and assign the object

    for (size_t i = 0; i < 5; i++) {
        cout << current->value << endl;  // use -> for pointer access
        
        if (!current->children.empty()) {
            for (const auto& child : current->children) {
                nextNodes.push_back(child);

            }

            int the_x=current->index[0];
            int the_y=current->index[0];
            get<1>(main_graph.gridNode[the_x][the_y]).setColor(color(0,0,255));
           std::vector<object> allObjects;
            for (const auto& row : main_graph.gridNode) {
                for (const auto& cell : row) {
                    allObjects.push_back(std::get<1>(cell));
                }
            }

            space s(allObjects);
            s.cameras.push_back(cam1);
            s.triggerCameraRayOptimized();
            // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
            std::filesystem::create_directories("Output");
            ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
            


            current->explored=true;
            current = current->children.at(0);  // move to first child
        } else {
            cout << "no childrens found";
            break;
        }
    }

}

void testGraph4()
{
    // the entended square grid
    size_t x=10,y=10;
    graph main_graph(x,y);
   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);

    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =10;
    for (size_t i = 0; i < grid_sizer; i++)
    {
        for (size_t j = 0; j < grid_sizer; j++)
        {
            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(main_graph.gridNode[i][j]) = obj;  
        }
    }

    
     graphNode* current = &(get<0>(main_graph.gridNode.at(0).at(0)));
     std::vector<graphNode*> nextNodes;
     nextNodes.push_back(current);

     // Create a space and assign the object

    for (size_t i = 0; i < 5; i++) {
        cout << current->value << endl;  // use -> for pointer access
        main_graph.step_dfs(1);   
        int the_x=current->index[0];
        int the_y=current->index[1];
        get<1>(main_graph.gridNode[the_x][the_y]).setColor(color(0,0,255));
        std::vector<object> allObjects;
        for (const auto& row : main_graph.gridNode) {
            for (const auto& cell : row) {
                allObjects.push_back(std::get<1>(cell));
            }
        }

        space s(allObjects);
        s.cameras.push_back(cam1);
        s.triggerCameraRayOptimized();
        // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
        std::filesystem::create_directories("Output/Pa");
        ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(i) + ".ppm");
        


        current->explored=true;
        current = current->children.at(0);  // move to first child
    } 
    

}


void testGraph5()
{
    // the entended square grid
    size_t x=3,y=3;
    graph main_graph(x,y);
   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);
    
    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =x;
    for (size_t i = 0; i < grid_sizer; i++)
    {

        for (size_t j = 0; j < grid_sizer; j++)
        {

            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(main_graph.gridNode[i][j]) = obj;  
        }
    }


     // Create a space and assign the object
    bool trigger_next_end = false;
    bool printOneMore = false;

    for (size_t i = 0; i < x * y + 4; i++) {

        if (trigger_next_end) {
            if (printOneMore) {
                std::cout << "Final render after trace_path()\n";
                break;
            }
            main_graph.trace_path();   // update graph
            printOneMore = true;       // allow 1 more render pass
        } else {
            if (main_graph.step_dfs(1)) {
                trigger_next_end = true;
            }
        }

        std::vector<object> allObjects = main_graph.getObjects();

        space s(allObjects);
        s.cameras.push_back(cam1);
        s.triggerCameraRayOptimized();

        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFile(
            s.cameras.at(0).getimage(),
            "Output/Step" + std::to_string(i) + ".ppm"
        );
    }

    

}

void testGraph6()
{
    // the entended square grid
    size_t x=10,y=10;
    graph main_graph(x,y);
   
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);
    
    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =x;
    for (size_t i = 0; i < grid_sizer; i++)
    {

        for (size_t j = 0; j < grid_sizer; j++)
        {

            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
            std::get<1>(main_graph.gridNode[i][j]) = obj;  
        }
    }


     // Create a space and assign the object
    bool trigger_next_end = false;
    bool printOneMore = false;

    for (size_t i = 0; i < x * y + 4; i++) {

        if (trigger_next_end) {
            if (printOneMore) {
                std::cout << "Final render after trace_path()\n";
                break;
            }
            main_graph.trace_path();   // update graph
            printOneMore = true;       // allow 1 more render pass
        } else {
            if (main_graph.step_dfs(1)) {
                trigger_next_end = true;
            }
        }

        std::vector<object> allObjects = main_graph.getObjects();

        space s(allObjects);
        s.cameras.push_back(cam1);
        s.triggerCameraRayOptimized();

        std::filesystem::create_directories("Output");
        ImageRenderer::renderToFile(
            s.cameras.at(0).getimage(),
            "Output/Step" + std::to_string(i) + ".ppm"
        );
    }

    // maybe run this to see it 
    // ffmpeg -framerate 4 -i Step%d.ppm -c:v libx264 -crf 18 -preset veryslow -pix_fmt yuv420p -r 30 output_video.mp4

    

}

void tttt()
{

     std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    size_t ratio = 5;
    unsigned int size = 2000 / ratio;
    double step = .01f * ratio;

    // camera config
    point camOrigin(-6, 3, -10);
    vec3 camYDirection(1, 0, 0);
    vec3 camXDirection(0, -1, 1);

    // mesh datat
    double scaling = 0.8;
    point offset = point(-5.5, 8, 0);
    vec3 axis(1, 1, 1);

    camera cam1(size, size, step, camOrigin, camXDirection, camYDirection, 1);
    object obj(primitive::plane, scaling, offset);

    // create a grid of objects
    vector<object> test;
    double offsetmultiplier = 2;
    size_t grid_sizer =10;
    for (size_t i = 0; i < grid_sizer; i++)
    {
        for (size_t j = 0; j < grid_sizer; j++)
        {
            object obj(primitive::plane, scaling, offset + point(scaling / 2, scaling / 2, scaling / 2) + point(j * offsetmultiplier, 0, i * offsetmultiplier));
            obj.setColor(color(255,0,0));
            test.push_back(obj);
        }
    }

    // Create a space and assign the object
    space s(test);
    s.cameras.push_back(cam1);
    s.triggerCameraRayOptimized();

    // ImageRenderer::renderToFile(stitchedImage, "Step"+std::to_string(i)+".ppm");
    std::filesystem::create_directories("Output");
    ImageRenderer::renderToFile(s.cameras.at(0).getimage(), "Output/Step" + std::to_string(0) + ".ppm");

    

   
}


int main(int argc, char const *argv[])
{
    // testintersection();
    // testimage()
    // testrayget();
    // testvec3();
    // testCamera();
    // testSpace();
    // testInterction();
    // testInterctionAlt();
    // testSpaceCamera();
    // testSpaceCameraCube();
    // testSpaceCameraCube1();
    // testSpaceCameraCube2();
    // testSpaceCameraCube3();
    // testFileLoad();
    // testMeshImportAndColoringDear();
    // testMeshImportAndColoringDhalia();
    // testMeshImportAndColoringSuzane();
    // split_rays();
    // split_raysThreads();
    // cubeTextureTest();
    // primitiveThreadTest();
    // split_raysThreadsCube();
    // cameraConfigTest();
    // generateVideo();
    // generateVideo1();
    // generateVideo2();
    // testPerspective();
    // testPerspectiveLoop();
    // testPerspectiveLoop1();
    // splitCamera();
    // splitCamera1();
    // splitCameraThreadingV2();
    // testraytracing();
    // testraytracing2();
    // testNonOptimizedrender();
    // testOptimizedrender();
    // testQuaternion();
    // testRotationOnMesh();
    // testRotationOnMeshVideo();
    // testRotationOnMeshVideo1();
    // testPerlin();
    //testGraph();
    //testGraph2();
    //testGraph3();
    //testGraph4();
    //testGraph5();
    testGraph6();
    //tttt();
    return 0;
}

// shortcut to collapse all : citrl + k + 0
// shortcut to expand all : citrl + k + j
// clean formating ctrl + k then -> ctrl + f
// clean formating ctrl + k then -> ctrl + k