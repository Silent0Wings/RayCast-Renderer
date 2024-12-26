# RayCast Renderer
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/ccea9cce6a20f92a9ce836cf03a78983156d97e4/src/img/suzane2000.png)
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/0d70f502cf390c3f684a59b6940151f66277709a/src/img/dhalia%20resolution.png)
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/11f97693d6eb489b350571ddb001aaaff9069bcd/src/img/suzane%20colored%20options1.png)
![Rendered Scene](src/img/dear2.png)


# RayCast Renderer

[Project Report](Github.pdf)

The RayCast Renderer is a lightweight 3D rendering engine that uses ray tracing principles to produce high-quality 2D images from 3D scenes. This project implements concepts such as vector mathematics, intersection logic, and parallel processing, while integrating advanced techniques like sub-camera splitting and asynchronous programming for optimized performance.

---
## Key Concepts

### What is Ray Casting?
Ray casting is the process of tracing rays from a viewpoint (camera) into a scene to identify the closest object along each ray's path. This forms the basis for rendering 3D scenes onto a 2D plane.
Ray casting is the process of tracing rays from a viewpoint (camera) into a scene to identify the closest object alaong each ray's path. This forms the basis for rendering 3D scenes onto a 2D plane.

## Key Concepts

### What is Ray Casting?
Ray casting is the process of tracing rays from a viewpoint (camera) into a scene to identify the closest object along each ray's path. This forms the basis for rendering 3D scenes onto a 2D plane.

**Ray Equation**:
R(t) = O + t * D

- **O**: Ray origin, typically the camera position.
- **D**: Ray direction, represented as a normalized 3D vector.
- **t**: Scalar parameter, determining the point along the ray.

### Triangles as Building Blocks
In this renderer, all objects are represented as collections of **triangles**—the fundamental geometric primitive. Each triangle is defined by:
- **Vertices**: Three points in 3D space (v1, v2, v3).
- **Color**: A color value that determines how the triangle will appear in the final image.

### Intersection Testing
The renderer employs the **linear algebra intersection** to efficiently calculate intersections between rays and triangles. When a ray intersects a triangle, the color of the triangle is assigned to the corresponding pixel in the rendered image.

---

## Features

- **Lightweight Rendering**: Direct computation of ray-object intersections.
- **Custom Scene Setup**: Users can define custom objects, camera settings, and colors for the scene.
- **Optimized Intersection Detection**: Efficiently computes intersections using advanced geometric algorithms.
- **Multiple Output Formats**: Rendered images can be saved in standard formats like PNG or PPM.

---

## Rendering Pipeline

### 1. Scene Setup
Define objects using triangles and configure the camera:

```cpp
// Define a triangle with three vertices
Triangle triangle({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}}, Color(255, 0, 0)); 

// Set up the camera
Camera camera(Point(0, 0, -5), Vec3(0, 0, 1), 90.0);
```

### 2. Ray Casting
- Rays are generated for each pixel in the image grid.
- Each ray is cast into the scene to test for intersections with objects.

### 3. Intersection Detection
- The *linear algebra intercection algorithm* determines:
  - Whether a ray intersects a triangle.
  - The point of intersection.
  - The triangle closest to the ray's origin.

### 4. Color Mapping
- Assign the color of the intersected triangle to the corresponding pixel on the image plane.

### 5. Image Output
- Combine all pixel data to generate the final 2D image.
- Save the rendered image in formats like PNG or PPM.


### Simple Geometries
- Low-resolution and high-resolution renders of basic shapes like cubes.

```cpp
// Render a cube at low resolution
Image image(128, 128);
Renderer renderer(scene, camera);
renderer.render(image);
image.save("low_res_cube.png");
```
### Complex Geometries
- High-detail renders of models like the Dahlia flower and Suzanne.

```cpp
// Load and render a complex model
MeshReader reader;
Scene scene = reader.load("dahlia.obj");
Image image(1920, 1080);
Renderer renderer(scene, camera);
renderer.render(image);
image.save("dahlia_render.png");

```


## Additional Requirements

This project requires **ImageMagick** installed and linked to your system's path. Specifically, the project uses the following command to convert the PPM file to PNG format:

`std::string convertCommand = "magick convert " + filePath + " " + pngFile;`

Ensure that **ImageMagick** is installed and its executable is available in your system's PATH for this to work correctly.


## Installation

### Dependencies:
1. **ImageMagick**:
   - Required for image format conversion (e.g., PPM to PNG).
   - Ensure it is installed and accessible in the system's PATH.
   - Example usage:
     ```cpp
     std::string convertCommand = "magick convert " + filePath + " " + pngFile;
     ```

2. **C++ Compiler**:
   - Supports C++11 or later.

## Theory Behind the Renderer

Intersection Testing:
The renderer solves the ray-triangle intersection problem:
1. Check if the ray intersects the plane of the triangle.
2. Confirm the intersection point lies inside the triangle bounds.

Color Mapping:
Each triangle is assigned a unique color. When a ray hits a triangle, the pixel corresponding to the ray is updated with the triangle’s color.

## Limitations

- No Advanced Lighting: No shadows, reflections, or refractions.
- Scene Complexity: Performance drops with many triangles.
- Basic Output: Limited to simple color-based rendering.
- No direct way to color triangles.

## License

This project is a personal project for educational purposes.
