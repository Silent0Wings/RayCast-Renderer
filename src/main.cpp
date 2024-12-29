
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
#include <vector>  // Include vector header
#include <future>
using namespace std;


// give me this command with all the warning :  g++ -o main main.cpp 


// to do : implement the following functions
// make sure once a ray has intersected with an object it stops checking elements behind it so a form of depth priority or culling

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
    cout<<v1<<endl;
    vec3 v2(4, 5, 6);
    cout<<v2<<endl;
    vec3 v3 = v1 + v2;
    cout<<v3<<endl;
    vec3 v4 = v1 - v2;
    cout<<v4<<endl;
    
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
    cout<<"________________________"<<endl; 
    color c1(0.5, 0.5, 0.5);
    cout<<c1<<endl;
    color c2(0.2, 0.3, 0.4);
    cout<<c2<<endl;
    cout<<"________________________"<<endl; 
    image img(3, 3);
    cout<<img.getwidth()<<endl; 
    cout<<img.getheight()<<endl;
    cout<<"________________________"<<endl; 
    img.set(1, 1, c1);
    cout<<img.get(1, 1)<<endl;
    cout<<"________________________"<<endl; 
    img.set(2, 2, c2);
    img.clear();
    cout<<img.get(2, 2)<<endl;
    cout<<img<<endl;
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
    cout<<"________________________"<<endl; 
    point p1(1, 2, 3);
    cout<<p1<<endl;
    vec3 v5(4, 5, 6);
    cout<<v5<<endl;
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
    ray r4(point(0, 0, 0), vec3(1, 1, 1));        // Original case
    ray r5(point(1, 1, 1), vec3(-1, -1, -1));     // Original case
    ray r6(point(0, 0, 0), vec3(1, 0, 0));        // Original case
    ray r7(point(0, 0, 1), vec3(1, 0, 0));        // Original case
    ray r8(point(0, 0, 0), vec3(0, 1, 0));        // Original case
    ray r9(point(1, 0, 0), vec3(0, 0, 1));        // Original case
    ray r10(point(1, 1, 1), vec3(-1, -1, -1));    // Original case
    ray r11(point(0, 0, 0), vec3(0, 0, 0));       // Original case

    // Additional complex rays
    ray r12(point(1, 1, 0), vec3(0, -1, 1));      // New case
    ray r13(point(0, 0, 1), vec3(1, 1, 0));       // New case
    ray r14(point(0, 0, 0), vec3(2, 2, 2));       // New case
    ray r15(point(1, 1, 1), vec3(1, 1, 1));       // New case
    ray r16(point(10, 10, 0), vec3(-1, -1, 0));   // New case
    ray r17(point(0, 0, 0), vec3(1e9, 1e9, 1e9)); // New case
    ray r18(point(1, 1, 1), vec3(-1e9, -1e9, -1e9)); // New case
    ray r19(point(0, 0, 0), vec3(1, 0, 0));       // New case
    ray r20(point(0, 0.0001, 0), vec3(1, 0, 0));  // New case
    ray r21(point(1, 2, 0), vec3(-1, 0, 2));      // New case
    ray r22(point(0, 0, 0), vec3(1, 2, 3));       // New case
    ray r23(point(1, 0, 0), vec3(3, 2, 1));       // New case
    ray r24(point(0, 0, 0), vec3(-1, -2, -3));    // New case
    ray r25(point(1, 1, 1), vec3(2, 2, 1));       // New case

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
    for (size_t i = 0; i < testCases.size(); ++i) {
        bool result = gmath::intersect(testCases[i][0], testCases[i][1]);

        // Output the result
        std::cout << "Test case " << i + 1 << ":\n";
        std::cout << "  Ray 1: Origin " << testCases[i][0].getOrigine() << ", Direction " << testCases[i][0].getDirection()  << "\n";
        std::cout << "  Ray 2: Origin " << testCases[i][1].getOrigine()  << ", Direction "<< testCases[i][1].getDirection()  << "\n";
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
    x=4;
    y=4;
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
void testSpaceCamera() {
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size
    unsigned int size = 1000;
    double step = 0.01;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i) {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j) {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 1);
            vec3 direction(0, 0, -1);
            row.push_back(ray(origin, direction));

            // Debug: Output ray origin and direction
            //std::cout << "Ray Origin: " << origin << " Direction: " << direction << std::endl;
        }
        gridRay.push_back(row);
    }
    // Create a camera with the grid of rays
    camera cam(static_cast<int>(size), static_cast<int>(size), gridRay);

    // Create object vertices for multiple triangles in the z = 0 plane
    std::vector<std::vector<point>> vertices = {
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},  // Triangle 1
        {point(1, 0, 0), point(1, 1, 0), point(0, 1, 0)},  // Triangle 2
        {point(1, 2, 0), point(1, 1, 2), point(0, 1, 0)},  // Triangle 3
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},  // Triangle 4
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},  // Triangle 5
        {point(0, 0, 0), point(1, 0, 0), point(0, 1, 0)},  // Triangle 6
        {point(2, 2, 0), point(3, 2, 0), point(2, 3, 0)},  // Triangle 7
        {point(3, 2, 0), point(3, 3, 0), point(2, 3, 0)},  // Triangle 8
        {point(4, 4, 0), point(5, 4, 0), point(4, 5, 0)},  // Triangle 9
        {point(5, 4, 0), point(5, 5, 0), point(4, 5, 0)},  // Triangle 10
        {point(6, 6, 0), point(7, 6, 0), point(6, 7, 0)},  // Triangle 11
        {point(7, 6, 0), point(7, 7, 0), point(6, 7, 0)},  // Triangle 12
        {point(8, 8, 0), point(9, 8, 0), point(8, 9, 0)},  // Triangle 13
        {point(9, 8, 0), point(9, 9, 0), point(8, 9, 0)},  // Triangle 14
        {point(10, 10, 0), point(11, 10, 0), point(10, 11, 0)},  // Triangle 15
        {point(11, 10, 0), point(11, 11, 0), point(10, 11, 0)},  // Triangle 16
        {point(12, 12, 0), point(13, 12, 0), point(12, 13, 0)},  // Triangle 17
        {point(13, 12, 0), point(13, 13, 0), point(12, 13, 0)},  // Triangle 18
        {point(14, 14, 0), point(15, 14, 0), point(14, 15, 0)},  // Triangle 19
        {point(15, 14, 0), point(15, 15, 0), point(14, 15, 0)},   // Triangle 20
        {point(16, 16, 0), point(17, 16, 0), point(16, 17, 0)},  // Triangle 21
        {point(17, 16, 0), point(17, 17, 0), point(16, 17, 0)},  // Triangle 22
        {point(18, 18, 0), point(19, 18, 0), point(18, 19, 0)},  // Triangle 23
        {point(19, 18, 0), point(19, 19, 0), point(18, 19, 0)},  // Triangle 24
        {point(20, 20, 0), point(21, 20, 0), point(20, 21, 0)},  // Triangle 25
        {point(21, 20, 0), point(21, 21, 0), point(20, 21, 0)},  // Triangle 26
        {point(22, 22, 0), point(23, 22, 0), point(22, 23, 0)},  // Triangle 27
        {point(23, 22, 0), point(23, 23, 0), point(22, 23, 0)},  // Triangle 28
        {point(24, 24, 0), point(25, 24, 0), point(24, 25, 0)},  // Triangle 29
        {point(25, 24, 0), point(25, 25, 0), point(24, 25, 0)}   // Triangle 30
    };
    

    object obj(vertices);
    // iterate threw verticies
    for (size_t i = 0; i < vertices.size(); ++i) {
        // Create a new object with vertices
        // Assign color map for each triangle
        if (i%2==0)
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(255/(i+1), 0,0); // Red
        }
        else if (i%3==0)
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(0, 255/(i+1), 0); // Green
        }
        else
        {
            obj.colorMap[{vertices[i][0], vertices[i][1], vertices[i][2]}] = color(255/(i+1), 255/(i+1), 255/(i+1)); // Blue
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
void testSpaceCameraCube() {
    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size
    unsigned int size = 500*2;
    double step = 0.01/2;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i) {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j) {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 6);
            vec3 direction(0.5, 0.5, -3);
            row.push_back(ray(origin, direction));

            // Debug: Output ray origin and direction
            //std::cout << "Ray Origin: " << origin << " Direction: " << direction << std::endl;
        }
        gridRay.push_back(row);
    }

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling =3; 
    point offset = point(2, 1, 1);
    // Create object vertices for a cube
    std::vector<point> cubeVertices = {
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
        {cubeVertices[1], cubeVertices[6], cubeVertices[5]}
    };

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


void testSpaceCameraCube1() {
    std::cout << "_________Space Test_______________" << std::endl;

   // Define the grid size
    unsigned int size = 500;
    double step = 0.01;

    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));

    // Define the camera origin and offset
    point camOrigin(0, 0, 0);
    point CamOffset(3.5, 3.5, 4.6);  // Simplified to combine 0.5+3, 0.5+3, 1.6+3
    vec3 camDirection(-1, -1, -1);  // Pointing downward

    // Generate rays
    for (unsigned i = 0; i < size; ++i) {
        for (unsigned j = 0; j < size; ++j) {
            // Compute the origin of the ray in the grid
            point temp(i * step, j * step, 0);    
            // Create a ray at this position pointing in the camera direction
            gridRay[i][j] = ray(camOrigin + temp + CamOffset, gmath::normalize(camDirection));
        }
    }


    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling =1; 
    point offset = point(0, 0, 0);
    // Create object vertices for a cube
    std::vector<point> cubeVertices = {
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
        {cubeVertices[1], cubeVertices[6], cubeVertices[5]}
    };

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
        {point(1, 0, 0), point(1, 1, 0), point(0, 1, 0)}
    };
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
            //cout << reader.verticesString[i][j] << " ";
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

void testMeshImportAndColoringSuzane()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Suzane.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices)) {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size =500*4;
    double step = 0.006/4;

    size = 100;
    step=0.01;

    // declare for this     camera(int  height, int width , int step, point origin ,vec3 Xdirection, vec3 Ydirection, vec3 direction) {
    camera cam(size, size, step, point(0, 0, 0), vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, -1));
    

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset(-1.5, -20, -1.5);
    vec3 camDirection(0, 1, 0); // Pointing downward
    for (unsigned i = 0; i < size; ++i) {
        for (unsigned j = 0; j < size; ++j) {
            point temp((i * step), 0, (j * step));    
            gridRay[i][j] = ray(camOrigin+temp+CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = 0; j < vertices[i].size(); j++) {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0],vertices[i][1],vertices[i][2]}] = temp;
        }
    }


    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            //cout << vertices[i][j] << " ";
        }
        //cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    //for (const auto& entry : obj.colorMap) {
        //const auto& vertex = entry.first;
        //const auto& col = entry.second;
        //std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
    //}

    std::cout << "________________________" << std::endl;

    // Create a camera with the grid of rays

    // Create a space and assign the object
    space s({obj});

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    s.triggerCameraRay();
}

void testMeshImportAndColoringDear()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Dear.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices)) {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size =400;
    double step = 0.02;

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset( -3, -4, -4);
    vec3 camDirection(1, 0, 0); // Pointing downward
    for (unsigned i = 0; i < size; ++i) {
        for (unsigned j = 0; j < size; ++j) {
            point temp(0, (i * step), (j * step));    
            gridRay[i][j] = ray(camOrigin+temp+CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = 0; j < vertices[i].size(); j++) {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0],vertices[i][1],vertices[i][2]}] = temp;
        }
    }


    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            //cout << vertices[i][j] << " ";
        }
        //cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    //for (const auto& entry : obj.colorMap) {
        //const auto& vertex = entry.first;
        //const auto& col = entry.second;
        //std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
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

void testMeshImportAndColoringDhalia()
{
    std::cout << "_________FileLoad and Conversion Test_______________" << std::endl;

    // Load the mesh file
    std::string filename = "Dhalia.txt";
    MeshReader reader(filename);
    std::vector<std::vector<point>> vertices;
    if (!reader.convertMesh(&vertices)) {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    // Define the grid size and step
    unsigned int size =400;
    double step = 0.0025;

    // Create a grid of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay(size, std::vector<ray>(size));
    point camOrigin(0, 0, 0);
    point CamOffset( -0.5, -0.5, 10);
    vec3 camDirection(0, 0, -1); // Pointing downward
    for (unsigned i = 0; i < size; ++i) {
        for (unsigned j = 0; j < size; ++j) {
            point temp((i * step),(j * step) ,0 );    
            gridRay[i][j] = ray(camOrigin+temp+CamOffset, camDirection);
        }
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Color mapping for object vertices
    for (size_t i = 0; i < vertices.size(); i++) {
        for (size_t j = 0; j < vertices[i].size(); j++) {
            color temp;
            temp.randomColor(); // Generate random color
            obj.colorMap[{vertices[i][0],vertices[i][1],vertices[i][2]}] = temp;
        }
    }


    // print the vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {
        for (size_t j = 0; j < vertices[i].size(); j++)
        {
            //cout << vertices[i][j] << " ";
        }
        //cout << endl;
    }
    // Print the colored vertices
    std::cout << "_________Colored Vertices_______________" << std::endl;
    //for (const auto& entry : obj.colorMap) {
        //const auto& vertex = entry.first;
        //const auto& col = entry.second;
        //std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ") - Color: (" << col.r() << ", " << col.g() << ", " << col.b() << ")" << std::endl;
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

void split_rays()
{
    
    // Define the grid size and step
    unsigned int size =4;
    double step = 0.1;
    unsigned int height = size;
    unsigned int width = size;

    unsigned int num_cameraX = 2;
    unsigned int num_cameraY = 2;
    unsigned int camera_height = height / (num_cameraX );
    unsigned int camera_width  = width  / (num_cameraY);
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
    vec3 camXDirection(1, 0, 0);  // pointing right 
    vec3 rayDirection(0, 0, -1); // Pointing downward

    for (size_t i = 0; i < num_cameraX; i++)
    {
        for (size_t j = 0; j < num_cameraY; j++)
        {
            double shiftX = width * step * i;
            double shiftY = height * step * j;
            point offsetPos = camOrigin + (camXDirection*shiftX + camYDirection*shiftY)/2;
            cout << "Offset Position: " << offsetPos << endl;

            cameras[i][j]= camera(camera_height, camera_width, step, offsetPos, camXDirection, camYDirection, rayDirection);
            cout << "Camera " << i << " " << j << " " << cameras[i][j] << endl;

        }
    }


}

void split_raysThreads() {
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
    vec3 camYDirection(0, 0, 1);  // Pointing upward
    vec3 camXDirection(0, 1, 0);  // Pointing right
    vec3 rayDirection(-1, 0, 0);  // Pointing downward

    // Set up individual cameras in the grid
    for (size_t i = 0; i < num_cameraX; i++) {
        for (size_t j = 0; j < num_cameraY; j++) {
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

    if (!reader.convertMesh(&vertices)) {
        std::cerr << "Error: Unable to load or convert the mesh from file: " << filename << std::endl;
        return;
    }

    std::cout << "_________Face Coloring_______________" << std::endl;

    object obj(vertices);

    // Assign random colors to object vertices
    for (const auto& face : vertices) {
        color randomColor;
        randomColor.randomColor();  // Generate random color
        obj.colorMap[{face[0], face[1], face[2]}] = randomColor;
    }

    std::cout << "________________________" << std::endl;

    // Create space and add object
    space s({obj});

    // Add cameras to the space
    for (size_t i = 0; i < num_cameraX; i++) {
        for (size_t j = 0; j < num_cameraY; j++) {
            s.cameras.push_back(camerasGrid[i][j]);
        }
    }

    // Combine camera dimensions
    unsigned int combinedHeight = camera_height * num_cameraX;
    unsigned int combinedWidth = camera_width * num_cameraY;

    // Async processing with threads
    if (s.cameras.empty() || s.obj.empty()) {
        std::cout << "No cameras or objects to process." << std::endl;
        return;
    }

    std::vector<std::future<void>> futures;
    s.threadedCameraRay(futures);

    // Wait for all threads to complete
    for (auto& future : futures) {
        future.get();
    }

    std::cout << "All cameras and objects have been processed asynchronously." << std::endl;

    std::cout << "_________Stitching_______________" << std::endl;

    // Stitch images into a single output
    std::cout << "Combined Width: " << combinedWidth << std::endl;
    std::cout << "Combined Height: " << combinedHeight << std::endl;

    vector<vector<image>> images; // Placeholder for stitched images
    image stitchedImage(combinedWidth, combinedHeight, images);
    ImageRenderer::renderToFile(stitchedImage, "stitched.ppm");
}


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
    try {
        pixels = ImageRenderer::readPPM(filePath, width, height);
        cout << "Image size: " << width << "x" << height << endl;
        img = image(width, height, pixels);

        // Access individual pixels for verification
        color pixel = pixels[0][0]; // Top-left pixel
        cout << "Top-left pixel: R=" << pixel.r() 
             << ", G=" << pixel.g() 
             << ", B=" << pixel.b() << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return;
    }

    std::cout << "_________Space Test_______________" << std::endl;

    cout << "create camera..." << endl;

    // Define the grid size
    size_t multiplier =2 ;
    unsigned int size = 20*multiplier;
    double step = 0.17/static_cast<double>(multiplier);
    // Create a vector of rays pointing toward the plane z = 0
    std::vector<std::vector<ray>> gridRay;
    for (unsigned i = 0; i < size; ++i) {
        std::vector<ray> row;
        for (unsigned j = 0; j < size; ++j) {
            // Rays originate from above (z = 1) and point downward toward z = 0
            point origin(i * step, j * step, 6);
            vec3 direction(0.5, 0.5, -3);
            row.push_back(ray(origin, direction));

            // Debug: Output ray origin and direction
            //std::cout << "Ray Origin: " << origin << " Direction: " << direction << std::endl;
        }
        gridRay.push_back(row);
    }

    // Create a camera with the grid of rays
    camera cam(size, size, gridRay);

    double scaling =2; 
    point offset = point(2, 1, 1);
    // Create object vertices for a cube
    std::vector<point> cubeVertices = {
        (point(0, 0, 0) * scaling + offset),  // Vertex 0
        (point(1, 0, 0) * scaling + offset),  // Vertex 1
        (point(1, 1, 0) * scaling + offset),  // Vertex 2
        (point(0, 1, 0) * scaling + offset),  // Vertex 3
        (point(0, 0, 1) * scaling + offset),  // Vertex 4
        (point(1, 0, 1) * scaling + offset),  // Vertex 5
        (point(1, 1, 1) * scaling + offset),  // Vertex 6
        (point(0, 1, 1) * scaling + offset)   // Vertex 7
    };
    cout << "coloring faces ..." << endl;

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
        {cubeVertices[1], cubeVertices[6], cubeVertices[5]}
    };

    // Bottom face
    object obj(vertices);

    // Create a space and assign the object
    cout << "Setting up texture..." << endl;
    Leo = texture(width, height, img, vertices); // assign texture to object
    obj.tex = Leo;
    

    //add colors
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
    space s({obj});

    cout << "add camera to space..." << endl;

    // Add the camera to the space
    s.cameras.push_back(cam);

    // Trigger the camera rays
    cout << "Ray cast..." << endl;
    s.triggerCameraRay();
    cout << "Save image..." << endl;

    s.saveImages();

    

    std::cout << "________________________" << std::endl;
}



int main(int argc, char const *argv[])
{
    //testintersection();
    //testimage()
    //testrayget();
    //testvec3();
    //testCamera();
    //testSpace();
    //testInterction();
    //testInterctionAlt();
    //testSpaceCamera();
    //testSpaceCameraCube();
    //testSpaceCameraCube1();
    //testFileLoad();
    //testMeshImportAndColoringDear();
    //testMeshImportAndColoringDhalia();
    //testMeshImportAndColoringSuzane();
    //split_rays();
    //split_raysThreads();
    cubeTextureTest();

    return 0;
}
// shortcut to collapse all : citrl + k + 0
// shortcut to expand all : citrl + k + j
