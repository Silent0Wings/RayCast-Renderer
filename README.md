# RayCast Renderer
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/0d70f502cf390c3f684a59b6940151f66277709a/src/img/Suzane.png)
![Rendered Scene](https://github.com/Silent0Wings/RayCast-Renderer/blob/0d70f502cf390c3f684a59b6940151f66277709a/src/img/dhalia%20resolution.png)


[Project Report](https://github.com/Silent0Wings/RayCast-Renderer/blob/346a89eef5504809262ef28ceef03915141df271/Github.pdf).

RayCast Renderer is a tool for generating 2D images from 3D scenes using ray casting. Rays are cast from a virtual camera into a 3D scene, checking for intersections with objects. The properties (e.g., colors) of intersected triangles are used to render the final image. This approach is simple, efficient, and avoids complex light interactions like reflections or refractions.

## Key Concepts

Ray Casting involves tracing rays from a viewpoint (camera) into a scene to determine the closest intersection with objects. Each ray represents a pixel in the output image.

Ray Equation:
R(t) = O + t * D
- O: Ray origin (camera position).
- D: Ray direction (a normalized vector).
- t: Scalar determining the point along the ray.

Triangles as Primitives:
Objects are represented as collections of triangles. Each triangle has:
- Vertices: Points in 3D space (v1, v2, v3).
- Color: A property used for rendering.

Intersection Testing:
The renderer uses the Möller-Trumbore Algorithm to efficiently detect ray-triangle intersections. If a ray intersects a triangle, the triangle’s color is assigned to the corresponding pixel.

## Features

- Lightweight Rendering: Direct ray-object intersection calculation.
- Custom Scene Setup: Supports user-defined objects and colors.
- Efficient Algorithm: Fast intersection testing with optimized math.
- Output: Saves rendered images in formats like PNG or PPM.

## Rendering Pipeline

1. Scene Setup:
   - Define the camera's position and orientation.
   - Add objects to the scene, represented as triangles.

2. Ray Casting:
   - Generate rays for each pixel in the output image.
   - Cast rays into the scene to find intersections.

3. Intersection Detection:
   - Use the Möller-Trumbore algorithm to compute intersections.

4. Color Mapping:
   - Assign triangle colors to pixels based on intersections.

5. Image Output:
   - Combine pixel data to produce the final image.

## Installation

1. Clone the repository:
   git clone https://github.com/Silent0Wings/RayCast-Renderer.git
   cd raycast-renderer
3. Build and run the project:
   build_run.bat

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

This project is a personal project for educational purpouses .
