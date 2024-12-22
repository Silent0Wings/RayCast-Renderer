# RayCast Renderer
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/ccea9cce6a20f92a9ce836cf03a78983156d97e4/src/img/suzane2000.png)
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/0d70f502cf390c3f684a59b6940151f66277709a/src/img/dhalia%20resolution.png)
# RayCast Renderer

[Project Report](Github.pdf)

RayCast Renderer is a simple yet powerful tool for generating 2D images from 3D scenes using the principles of ray casting. It projects rays from a virtual camera into a defined 3D scene, checking for intersections with geometric objects like triangles. The intersected objects' properties, such as color, are used to render the final 2D image. This method is computationally efficient and avoids handling complex light interactions, such as reflections, refractions, or shadows.

---

## Key Concepts

### What is Ray Casting?
Ray casting is the process of tracing rays from a viewpoint (camera) into a scene to identify the closest object along each ray's path. This forms the basis for rendering 3D scenes onto a 2D plane.

**Ray Equation**:
\[
R(t) = O + t \cdot D
\]
- **O**: Ray origin, typically the camera position.
- **D**: Ray direction, represented as a normalized 3D vector.
- **t**: Scalar parameter, determining the point along the ray.

### Triangles as Building Blocks
In this renderer, all objects are represented as collections of **triangles**—the fundamental geometric primitive. Each triangle is defined by:
- **Vertices**: Three points in 3D space (\(v_1, v_2, v_3\)).
- **Color**: A color value that determines how the triangle will appear in the final image.

### Intersection Testing
The renderer employs the **Möller-Trumbore Algorithm** to efficiently calculate intersections between rays and triangles. When a ray intersects a triangle, the color of the triangle is assigned to the corresponding pixel in the rendered image.

---

## Features

- **Lightweight Rendering**: Direct computation of ray-object intersections.
- **Custom Scene Setup**: Users can define custom objects, camera settings, and colors for the scene.
- **Optimized Intersection Detection**: Efficiently computes intersections using advanced geometric algorithms.
- **Multiple Output Formats**: Rendered images can be saved in standard formats like PNG or PPM.

---

## Rendering Pipeline

### 1. Scene Setup
- Define the **camera**:
  - Position: The origin point of the rays.
  - Orientation: Determines the direction of the rays.
- Add objects to the scene:
  - Represent objects as collections of triangles, each with specified vertices and colors.

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

3. **Additional Requirement**: 
   This project requires **ImageMagick** installed and linked to your system's path. 
   Specifically, the project uses the following command to convert the PPM file to PNG format:
   
   `std::string convertCommand = "magick convert " + filePath + " " + pngFile;`
   
   Ensure that **ImageMagick** is installed and its executable is available in your system's PATH for this to work correctly.


## Installation

### Prerequisites
- **Compiler**: A C++ compiler supporting C++11 or later.
- **ImageMagick**: Ensure ImageMagick is installed and added to your system path. The renderer relies on `magick convert` for output image processing.


## Usage

1. Scene Setup:
   Define a scene with objects:
   Scene scene;
   scene.add_object(Triangle({{0, 0, 0}, {1, 0, 0}, {0, 1, 0}}, "red"));

2. Rendering:
   Create a renderer and generate the output:
   ImageRenderer renderer(scene);
   renderer.render("output.png", 800, 600);

3. View Results:
   Check the output image (e.g., output.png) in your directory.

## Theory Behind the Renderer

Intersection Testing:
The renderer solves the ray-triangle intersection problem:
1. Check if the ray intersects the plane of the triangle.
2. Confirm the intersection point lies inside the triangle bounds using barycentric coordinates.

Color Mapping:
Each triangle is assigned a unique color. When a ray hits a triangle, the pixel corresponding to the ray is updated with the triangle’s color.

## Limitations

- No Advanced Lighting: No shadows, reflections, or refractions.
- Scene Complexity: Performance drops with many triangles.
- Basic Output: Limited to simple color-based rendering.
- No direct way to color triangles

## License

This project is a personal project for educational purposes.
