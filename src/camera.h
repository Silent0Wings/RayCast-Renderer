/**
 * @file camera.h
 * @brief Defines the camera class for camera operations.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "ray.h"
#include "object.h"
#include "point.h"
#include <optional>
using namespace std;

class camera
{
private:
    vector<vector<ray>> gridRay;
    unsigned int width;
    unsigned int height;
    color defaultColor;
    image img;

    /**
     * @brief Unified grid builder. Handles both orthographic and perspective.
     *
     * Orthographic mode:  perspectiveScale <= 1  OR  perspectiveForce == 0
     * Perspective mode:   perspectiveScale >  1  AND perspectiveForce != 0
     */
    void constructRay(unsigned int h, unsigned int w, double step, point origin,
                      vec3 xDir, vec3 yDir, vec3 rayDir,
                      double perspectiveScale, double perspectiveForce)
    {
        vec3 xd = gmath::normalize(xDir);
        vec3 yd = gmath::normalize(yDir);
        vec3 rd = gmath::normalize(rayDir);

        // 1. Build base orthographic grid (near plane)
        for (unsigned i = 0; i < h; ++i)
        {
            point row = origin + yd * (i * step);
            for (unsigned j = 0; j < w; ++j)
            {
                point p = row + xd * (j * step);
                gridRay[i][j] = ray(p, rd);
            }
        }

        // 2. If perspective requested, converge rays toward a far plane
        if (perspectiveScale > 1.0 && perspectiveForce != 0.0)
        {
            double sStep = step * perspectiveScale;

            // Build scaled (far) plane origins
            vector<vector<point>> scaled(h, vector<point>(w));
            for (unsigned i = 0; i < h; ++i)
            {
                point row = origin + yd * (i * sStep);
                for (unsigned j = 0; j < w; ++j)
                {
                    scaled[i][j] = row + xd * (j * sStep);
                }
            }

            // Recenter scaled plane so both share the same center pixel
            point cBase = gridRay[h / 2][w / 2].getOrigine();
            point cScale = scaled[h / 2][w / 2];
            vec3 shift = cBase - cScale;

            for (unsigned i = 0; i < h; ++i)
                for (unsigned j = 0; j < w; ++j)
                    scaled[i][j] = scaled[i][j] + shift;

            // Push far plane back along view direction
            for (unsigned i = 0; i < h; ++i)
                for (unsigned j = 0; j < w; ++j)
                    scaled[i][j] = scaled[i][j] + rd * perspectiveForce;

            // Redirect each ray toward its corresponding far-plane point
            for (unsigned i = 0; i < h; ++i)
            {
                for (unsigned j = 0; j < w; ++j)
                {
                    vec3 newDir = gmath::normalize(scaled[i][j] - gridRay[i][j].getOrigine());
                    gridRay[i][j] = ray(gridRay[i][j].getOrigine(), newDir);
                }
            }
        }
    }

public:
    /* --------------------------------------------------------------
       Constructors
       -------------------------------------------------------------- */

    camera() : gridRay(), width(0), height(0), defaultColor(), img() {}

    camera(const camera &other) = default;

    // Delegating convenience constructor for square cameras
    camera(int h) : camera(h, h) {}

    /**
     * @brief Unified constructor: orthographic by default, perspective when
     *        perspectiveScale > 1 and perspectiveForce != 0.
     *
     * @param h                Image height in pixels (rows)
     * @param w                Image width in pixels (columns)
     * @param step             Grid spacing in world units
     * @param origin           World-space anchor for the grid
     * @param xDir             Axis along the width  (columns)
     * @param yDir             Axis along the height (rows)
     * @param rayDir           Base ray direction (view axis)
     * @param perspectiveScale Multiplier for step on the far plane (>1 = perspective)
     * @param perspectiveForce Distance to push the far plane along rayDir
     */
    camera(int h, int w,
           double step = 1.0,
           point origin = point(0, 0, 0),
           vec3 xDir = vec3(1, 0, 0),
           vec3 yDir = vec3(0, 1, 0),
           vec3 rayDir = vec3(0, 0, -1),
           double perspectiveScale = 1.0,
           double perspectiveForce = 0.0)
    {
        if (w <= 0 || h <= 0 || step <= 0)
        {
            throw std::invalid_argument("Camera width, height and step must be positive");
        }

        width = static_cast<unsigned int>(w);
        height = static_cast<unsigned int>(h);

        gridRay.resize(height, vector<ray>(width));

        constructRay(height, width, step, origin,
                     xDir, yDir, rayDir,
                     perspectiveScale, perspectiveForce);

        img = image(width, height);
    }

    // Constructor from an existing ray grid
    camera(int w, int h, vector<vector<ray>> g)
        : gridRay(std::move(g)),
          width(static_cast<unsigned int>(w)),
          height(static_cast<unsigned int>(h)),
          defaultColor(),
          img(image(w, h))
    {
        if (w <= 0 || h <= 0)
            throw std::invalid_argument("Camera width and height must be positive");
    }

    /* --------------------------------------------------------------
       Geometry helpers
       -------------------------------------------------------------- */

    void recenterTo(const point &desiredCenter)
    {
        if (height == 0 || width == 0)
            return;

        point currentCenter = get(width / 2, height / 2).getOrigine();

        vec3 offset(
            desiredCenter.get_x() - currentCenter.get_x(),
            desiredCenter.get_y() - currentCenter.get_y(),
            desiredCenter.get_z() - currentCenter.get_z());

        for (unsigned i = 0; i < height; ++i)
        {
            for (unsigned j = 0; j < width; ++j)
            {
                ray r = get(j, i);
                set(j, i, ray(r.getOrigine() + offset, r.getDirection()));
            }
        }
    }

    point getOrigin() const
    {
        if (height == 0 || width == 0)
            return point(0, 0, 0); // or throw, depending on your style

        return get(width / 2, height / 2).getOrigine();
    }

    /* --------------------------------------------------------------
       Getters / Setters
       -------------------------------------------------------------- */

    unsigned int getwidth() const { return width; }
    unsigned int getheight() const { return height; }
    image getimage() const { return img; }

    vector<vector<ray>> getGridRay() { return gridRay; }

    ray get(unsigned int x, unsigned int y) const
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument(
                "Camera::get(): out of bounds. x: " + to_string(x) +
                " | y: " + to_string(y));
        }
        return gridRay[y][x];
    }

    void set(unsigned int x, unsigned int y, const ray &r)
    {
        if (constrain(x, y))
        {
            throw std::invalid_argument(
                "Camera::set(): out of bounds. x: " + to_string(x) +
                " | y: " + to_string(y));
        }
        gridRay[y][x] = r;
    }

    void setColor(unsigned int x, unsigned int y, const color &c)
    {
        if (img.constrain(x, y))
        {
            throw std::invalid_argument(
                "Camera::setColor(): out of bounds. x: " + to_string(x) +
                " | y: " + to_string(y));
        }
        img.set(x, y, c);
    }

    void setRay(vector<vector<ray>> g) { gridRay = std::move(g); }
    void setDefaultColor(const color &c) { defaultColor = c; }

    void clear()
    {
        for (unsigned i = 0; i < height; ++i)
            for (unsigned j = 0; j < width; ++j)
                img.set(i, j, defaultColor);
    }

    bool constrain(unsigned int x, unsigned int y) const
    {
        return (x >= width || y >= height);
    }

    void resize(unsigned int new_width, unsigned int new_height)
    {
        // Stub: original implementation was incomplete
        width = new_width;
        height = new_height;
        gridRay.resize(height, vector<ray>(width));
        img = image(width, height);
    }

    /* --------------------------------------------------------------
       Operators
       -------------------------------------------------------------- */

    friend std::ostream &operator<<(std::ostream &os, const camera &c)
    {
        os << "Ray( width : " << c.getwidth()
           << ", height : " << c.getheight() << ")\n";
        for (unsigned i = 0; i < c.getheight(); ++i)
        {
            for (unsigned j = 0; j < c.getwidth(); ++j)
                os << c.gridRay[i][j] << " | ";
            os << "\n";
        }
        return os;
    }

    bool operator==(const camera &other) const
    {
        if (width != other.width || height != other.height)
            return false;
        for (unsigned i = 0; i < height; ++i)
            for (unsigned j = 0; j < width; ++j)
                if (gridRay[i][j] != other.gridRay[i][j])
                    return false;
        return true;
    }

    bool operator!=(const camera &other) const { return !(*this == other); }

    /* --------------------------------------------------------------
       Rendering
       -------------------------------------------------------------- */

    // In cameraToImage:
    void cameraToImage(const object &obj)
    {
        for (unsigned i = 0; i < height; ++i)
        {
            for (unsigned j = 0; j < width; ++j)
            {
                auto &ray = gridRay[i][j];

                if (!gmath::intersectRaySphere(ray, obj.center, obj.sphereRadius))
                    continue;

                double bestDist = ray.hasLastHit() ? ray.getLastHitDistance() : std::numeric_limits<double>::infinity();

                if (!obj.boundingGrid)
                {
                    bool hit = getPixelColor(i, j, obj, ray, bestDist);
                    if (hit)
                        ray.setLastHitDistance(bestDist);
                    continue;
                }

                const auto &grid = *obj.boundingGrid;

                const auto visitedCubes = grid.TraverseRay(ray.getOrigine(), ray.getDirection());
                bool hit = false;
                for (const auto &[idx, cubeDist] : visitedCubes)
                {
                    if (cubeDist > bestDist)
                        break;

                    const auto &entry = grid.At(idx);
                    if (entry.data.triples.empty())
                        continue;

                    hit = getPixelColor(i, j, obj, entry.data.triples, ray, bestDist) || hit;
                }

                if (hit)
                    ray.setLastHitDistance(bestDist);
            }
        }
    }

    /* --------------------------------------------------------------
       Pixel helpers (private implementation)
       -------------------------------------------------------------- */

    // No-grid, full-object version that shares bestDist with the caller
    bool getPixelColor(
        unsigned int i,
        unsigned int j,
        object obj,
        ray r1,
        double &bestDist,
        bool combine = false)
    {
        bool hasTexture = !obj.tex.empty();
        bool hit = false;

        for (auto const &x : obj.colorMap)
        {
            std::array<point, 3> tri = {x.first[0], x.first[1], x.first[2]};

            std::optional<point> val = gmath::intersectRayTriangle(r1, tri.data());
            if (!val)
                continue;

            double d = gmath::distance(r1.getOrigine(), *val);
            if (d >= bestDist)
                continue;

            hit = true;
            bestDist = d;

            if (hasTexture)
            {
                color texel = obj.tex.get(i, j);
                img.set(i, j, combine ? (x.second / 10 + texel / 2) : texel);
            }
            else
            {
                img.set(i, j, x.second);
            }
        }

        return hit;
    }

    // Grid-cell version (already correct)
    bool getPixelColor(
        unsigned int i,
        unsigned int j,
        object obj,
        const std::vector<std::array<point, 3>> &tris,
        ray r1,
        double &bestDist,
        bool combine = false)
    {
        bool hasTexture = !obj.tex.empty();
        bool hit = false;

        for (const auto &tri : tris)
        {
            std::optional<point> val = gmath::intersectRayTriangle(r1, tri.data());
            if (!val)
                continue;

            double d = gmath::distance(r1.getOrigine(), *val);
            if (d >= bestDist)
                continue;

            hit = true;
            bestDist = d;

            if (hasTexture)
            {
                color texel = obj.tex.get(i, j);
                img.set(i, j, combine ? (obj.colorMap.at(tri) / 10 + texel / 2) : texel);
            }
            else
            {
                 img.set(i, j, obj.colorMap.at(tri));


                /*
                vec3 n = gmath::normalVector(tri[0], tri[1], tri[2]);
                color c(0, 0, 0);
                gmath::normalOrientationColor(n, c);
                img.set(i, j, c);
                */
            }
        }

        return hit;
    }
    /* --------------------------------------------------------------
       Split / Merge
       -------------------------------------------------------------- */

public:
    vector<camera> splitCamera(camera &c, const size_t split)
    {
        if (split <= 1 || c.getheight() == 0 || c.getwidth() == 0)
            return {camera()};

        size_t total = c.getheight() * c.getwidth();
        if (total <= split)
            return {camera()};

        vector<ray> linear;
        linear.reserve(total);
        for (auto const &row : c.gridRay)
            linear.insert(linear.end(), row.begin(), row.end());

        size_t chunk = linear.size() / split;
        if (chunk == 0)
            return {camera()};

        vector<vector<ray>> splits;
        vector<ray> temp;
        for (size_t i = 0; i < linear.size(); ++i)
        {
            if (temp.size() >= chunk && splits.size() < split - 1)
            {
                splits.push_back(std::move(temp));
                temp.clear();
            }
            temp.push_back(linear[i]);
        }
        if (!temp.empty())
            splits.push_back(std::move(temp));

        vector<camera> group;
        for (auto &chunkRays : splits)
        {
            camera cam;
            cam.width = static_cast<unsigned int>(chunkRays.size());
            cam.height = 1;
            cam.setRay({std::move(chunkRays)});
            cam.img = image(cam.height, cam.width);
            group.push_back(std::move(cam));
        }
        return group;
    }

    image construct_split(vector<camera> v, size_t h, size_t w)
    {
        vector<color> linear;
        for (auto const &cam : v)
        {
            auto px = cam.getimage().getPixels();
            if (px.size() != 1)
            {
                cout << "this can only construct linear images" << endl;
                return image();
            }
            linear.insert(linear.end(), px[0].begin(), px[0].end());
        }

        vector<vector<color>> finalImg;
        vector<color> row;
        for (auto const &c : linear)
        {
            if (row.size() >= w)
            {
                finalImg.push_back(std::move(row));
                row.clear();
            }
            row.push_back(c);
        }
        if (!row.empty())
            finalImg.push_back(std::move(row));

        if (finalImg.size() != h || finalImg.empty() || finalImg[0].size() != w)
            return image();

        return image(static_cast<unsigned int>(h), static_cast<unsigned int>(w), finalImg);
    }
};

#endif // CAMERA_H