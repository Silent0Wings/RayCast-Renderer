#include "vec3.h"
#include "point.h"
#include "ray.h"
#include "gmath.h"
#include <cmath>
#include <math.h>
#include "Hit.h"

double gmath::dot(const vec3 &vec1, const vec3 &vec2)
{
    return vec1.x() * vec2.x() + vec1.y() * vec2.y() + vec1.z() * vec2.z();
}

vec3 gmath::cross(const vec3 &vec1, const vec3 &vec2)
{
    return vec3(vec1.y() * vec2.z() - vec1.z() * vec2.y(),
                vec1.z() * vec2.x() - vec1.x() * vec2.z(),
                vec1.x() * vec2.y() - vec1.y() * vec2.x());
}

double gmath::length(const vec3 &vec)
{
    return sqrt(vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
}

vec3 gmath::normalize(const vec3 &vec)
{
    double len = length(vec);
    if (len == 0)
    {
        throw std::invalid_argument("gmath::normalize():Division by zero is not allowed");
    }
    return vec3(vec.x() / len, vec.y() / len, vec.z() / len);
}

double gmath::distance(const vec3 &p1, const vec3 &p2)
{
    double tempValue = length(p2 - p1);
    if (tempValue < 0)
    {
        tempValue *= -1;
    }

    return tempValue;
}

double gmath::angleBetween(const vec3 &vec1, const vec3 &vec2)
{
    return acos(dot(vec1, vec2) / (length(vec1) * length(vec2)));
}
double gmath::angleBetweenDegree(const vec3 &vec1, const vec3 &vec2)
{
    double rad = acos(dot(vec1, vec2) / (length(vec1) * length(vec2)));
    return 180 * rad / pi;
}
double gmath::radToDegree(double rad)
{
    return (180.0 * rad) / pi;
}

double gmath::DegreeToRad(double deg)
{
    return (deg * pi) / 180.0;
}

vec3 gmath::translateVec(const vec3 &vec, const vec3 &offset)
{
    return vec3(vec.x() + offset.x(), vec.y() + offset.y(), vec.z() + offset.z());
}

point gmath::translatePoint(const point &p, const vec3 &offset)
{
    return point(p.x() + offset.x(), p.y() + offset.y(), p.z() + offset.z());
}

point gmath::translatePointFactor(const point &p, const vec3 &offset, double t)
{
    return point(p.x() + offset.x() * t, p.y() + offset.y() * t, p.z() + offset.z() * t);
}
bool gmath::intersect(const ray &p, const ray &offset)
{
    vec3 dir1 = p.getDirection();
    vec3 dir2 = offset.getDirection();
    point p1 = p.getOrigine();
    point p2 = offset.getOrigine();

    // Case 1: Both rays have zero direction vectors
    if (dir1 == vec3::zero() && dir2 == vec3::zero())
    {
        return p1 == p2; // They intersect only if their origins are the same
    }

    // Case 2: r1 is a point, check if it lies on r2
    if (dir1 == vec3::zero())
    {
        vec3 toPoint = p1 - p2;
        return cross(toPoint, dir2) == vec3::zero() && dot(toPoint, dir2) >= 0;
    }

    // Case 3: r2 is a point, check if it lies on r1
    if (dir2 == vec3::zero())
    {
        vec3 toPoint = p2 - p1;
        return cross(toPoint, dir1) == vec3::zero() && dot(toPoint, dir1) >= 0;
    }

    // Case 4: Both rays have non-zero direction vectors
    vec3 v1 = normalize(dir1);
    vec3 v2 = normalize(dir2);
    vec3 v3 = p2 - p1;

    // Check if directions are parallel
    if (cross(v1, v2) == vec3::zero())
    {
        // Check if origins lie on the same line
        return cross(v3, v1) == vec3::zero();
    }

    // Directions are not parallel: Check if rays intersect
    double denom = length(cross(v1, v2));
    if (denom == 0)
    {
        return false; // Safety check
    }

    double t1 = dot(cross(v3, v2), cross(v1, v2)) / denom;
    double t2 = dot(cross(v3, v1), cross(v1, v2)) / denom;

    // Validate intersection points are on the rays
    return t1 >= 0 && t2 >= 0;
}

point *gmath::intersectLocation(const ray &r1, const ray &r2)
{
    vec3 v1 = normalize(r1.getDirection());
    vec3 v2 = normalize(r2.getDirection());
    point p1 = r1.getOrigine();
    point p2 = r2.getOrigine();

    vec3 v3 = p2 - p1;

    // Check if directions are parallel
    if (cross(v1, v2) == vec3::zero())
    {
        // Check if origins lie on the same line
        if (cross(v3, v1) == vec3::zero())
        {
            // Rays are collinear (intersection everywhere or nowhere)
            return nullptr; // No specific intersection point
        }
        return nullptr; // Parallel but not intersecting
    }

    // Compute intersection using parametric equations
    vec3 cross_v1v2 = cross(v1, v2);
    double denom = length(cross_v1v2);

    if (denom == 0)
    {
        return nullptr; // No intersection (shouldn't reach here if not parallel)
    }

    double t1 = dot(cross(v3, v2), cross_v1v2) / denom;
    double t2 = dot(cross(v3, v1), cross_v1v2) / denom;

    // Validate intersection points are on the rays
    if (t1 < 0 || t2 < 0)
    {
        return nullptr; // Intersection occurs "behind" the rays
    }

    // Compute the intersection location
    point *intersection = new point(p1 + v1 * t1); // Dynamically allocate the point
    return intersection;
}

point *gmath::intersectRayTriangle(const ray &r, const point triangle[3])
{
    const point &A = triangle[0];
    const point &B = triangle[1];
    const point &C = triangle[2];

    // Step 1: Early direction check (ray must point toward triangle)
    point rayOrigin = r.getOrigine();
    point shiftedOrigin = r.get(epsilon);
    bool movingToward = false;

    for (int i = 0; i < 3; ++i)
    {
        double distFromOrigin = gmath::distance(rayOrigin, triangle[i]);
        double distFromShifted = gmath::distance(shiftedOrigin, triangle[i]);
        if (distFromShifted < distFromOrigin)
        {
            movingToward = true;
            break;
        }
    }

    if (!movingToward)
        return nullptr;

    // Step 2: Plane intersection
    vec3 AB = B - A;
    vec3 AC = C - A;
    vec3 normal = cross(AB, AC);

    vec3 dir = r.getDirection();
    point origin = r.getOrigine();

    double denominator = dot(normal, dir);
    if (std::abs(denominator) < 1e-6)
        return nullptr; // Ray is parallel to triangle's plane

    double t = dot(normal, A - origin) / denominator;
    if (t < 0)
        return nullptr; // Intersection is behind ray

    point P = origin + dir * t;

    // Step 3: Barycentric coordinate test
    vec3 AP = P - A;

    double dot00 = dot(AB, AB);
    double dot01 = dot(AB, AC);
    double dot02 = dot(AB, AP);
    double dot11 = dot(AC, AC);
    double dot12 = dot(AC, AP);

    double denom = dot00 * dot11 - dot01 * dot01;
    if (std::abs(denom) < 1e-6)
        return nullptr;

    double u = (dot11 * dot02 - dot01 * dot12) / denom;
    double v = (dot00 * dot12 - dot01 * dot02) / denom;

    if (u >= 0 && v >= 0 && (u + v <= 1))
        return new point(P);

    return nullptr;
}

Hit *gmath::intersect3dHit(const ray &r1, const point arr[3])
{
    // this is a direction check to see if the ray is moving towards any of the triangle vertices or not
    int towardsTriangleVerticesCount = 0;
    for (size_t i = 0; i < 3; i++)
    {
        point p1 = arr[i];
        point rayOrigin = r1.getOrigine();
        point translatedRayOrigin = r1.get(epsilon); // using a value that is bigger than the cordinate of the object in the space wwould cause problems
        // say a cube is at 0,0,0 and the ray is at 0,0,0.00001, the ray would intersect the cube at 0,0,0

        double distance1 = gmath::distance(rayOrigin, p1);
        double distance2 = gmath::distance(translatedRayOrigin, p1);
        double difference = distance1 - distance2;
        if (difference > 0) // if the difference is positive, the ray is moving towards from the triangle
        {
            towardsTriangleVerticesCount++;
            break; // since only one is enough if we need to increse to two verticies then we can remove the break
        }
        else if (difference < 0)
        { // if the difference is negative, the ray is moving away from the triangle
          // No action needed
        }
        else
        { // if the difference is zero, the ray isn't moving at all
          // No action needed
        }
    }

    if (towardsTriangleVerticesCount >= 1) // if the ray is moving towards at least one of the triangle vertices, then it likely intersects
    {

    } // if the ray is not moving towards any of the triangle vertices, then it likely doesnt intersect
    else
    {
        return nullptr;
    }

    // Triangle edges
    vec3 edge1 = arr[1] - arr[0]; // Edge AB
    vec3 edge2 = arr[2] - arr[0]; // Edge AC

    // Normal vector of the triangle's plane
    vec3 n = cross(edge1, edge2);

    // Ray direction and origin
    vec3 d = r1.getDirection(); // Ray direction
    point o = r1.getOrigine();  // Ray origin

    // Check if the ray is parallel to the plane
    double denominator = dot(n, d);
    if (std::abs(denominator) < 1e-6)
    {
        return nullptr; // No intersection (ray is parallel to the plane)
    }

    // Compute the intersection parameter t
    double t = dot(n, arr[0] - o) / denominator;

    // Check if the intersection is valid (t >= 0 ensures it's in front of the ray origin)
    if (t < 0)
    {
        return nullptr; // No valid intersection (intersection behind the ray origin)
    }

    // Calculate the intersection point
    point P = o + d * t;

    // Check if the point lies inside the triangle using barycentric coordinates
    vec3 toPoint = P - arr[0]; // Vector from vertex A to the intersection point P

    // Compute dot products for barycentric coordinates
    double dot00 = dot(edge1, edge1);
    double dot01 = dot(edge1, edge2);
    double dot02 = dot(edge1, toPoint);
    double dot11 = dot(edge2, edge2);
    double dot12 = dot(edge2, toPoint);

    // Compute barycentric coordinates
    double denom = dot00 * dot11 - dot01 * dot01;
    if (std::abs(denom) < 1e-6)
    {
        return nullptr; // Degenerate triangle (no intersection)
    }

    double u = (dot11 * dot02 - dot01 * dot12) / denom;
    double v = (dot00 * dot12 - dot01 * dot02) / denom;

    // Ensure the intersection point lies inside the triangle
    if (u >= 0 && v >= 0 && (u + v) <= 1)
    {
        // Return the intersection point dynamically allocated

        /*
            point hitPoint;
            vec3 normal;
            double angle;
            vec3 incoming;
            vec3 outgoing;
        */
        vec3 outgoing = vec3(reflectorVector(d, n));
        double angle = angleBetweenDegree(d, outgoing);
        return new Hit(P, n, angle, d, outgoing);
    }
    else
    {
        return nullptr; // Intersection point is outside the triangle
    }
}

point *gmath::intersect3d2(const ray &r1, const point arr[4])
{
    // Check if the ray intersects the first triangle
    point arr1[3] = {arr[0], arr[1], arr[2]};
    // Check if the ray intersects the second triangle
    point arr2[3] = {arr[0], arr[2], arr[3]};

    point *p1 = intersectRayTriangle(r1, arr1);
    point *p2 = intersectRayTriangle(r1, arr2);

    // Determine the intersection point
    if (p1 == nullptr && p2 == nullptr)
    {
        return nullptr; // No intersection
    }
    else if (p1 == nullptr)
    {
        return p2; // Return the intersection point from the second triangle
    }
    else if (p2 == nullptr)
    {
        return p1; // Return the intersection point from the first triangle
    }
    else
    {
        // Both triangles intersect, return the closest intersection point
        double d1 = distance(r1.getOrigine(), *p1);
        double d2 = distance(r1.getOrigine(), *p2);
        return d1 < d2 ? p1 : p2;
    }
}

bool gmath::intersectRaySphere(const ray &r1, const point center, const double radius)
{

    // (x-h)^2 +(x-k)^2+(x-l)^2= Radius^2
    // h=x , k=y , l=z the center of the sphere
    // now a point along the ray is p= r0+rd*t
    // and the perpindicular projection of the origine of the sphere on the line represented by the ray is P at time  t calculared as so:
    // t=dot(s-r0,rd))

    double t = dot(center - r1.getOrigine(), r1.getDirection());
    point P = r1.get(t);

    // if the distance between t and the center of the sphere is > to the radius then it is outisde and there is no possible intersection
    if (distance(P, center) > radius) // then the intersection never happens since the line is outside of the sphere
    {
        return false;
    }
    else if (distance(P, center) == radius) // then the intersection happens at one point
    {
        return true;
    }
    else // intersection happens at 2 points
    {
        return true;
    }
}

double gmath::magnitude(const vec3 v)
{
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}

vec3 *gmath::reflectorVector(const vec3 incoming, const vec3 normal)
{
    vec3 n = normalize(normal);
    double dotP = dot(incoming, normal);
    // r=incoming−2(incoming⋅n)n
    vec3 outgoing = incoming - n * 2 * (dotP);
    return new vec3(outgoing);
}

vec3 gmath::rotate(const vec3 &vec, const vec3 &axis, double angle)
{
    double s = sin(angle);
    double c = cos(angle);
    double x = axis.x();
    double y = axis.y();
    double z = axis.z();
    double x2 = x * x;
    double y2 = y * y;
    double z2 = z * z;
    double xy = x * y;
    double xz = x * z;
    double yz = y * z;
    double xs = x * s;
    double ys = y * s;
    double zs = z * s;
    double one_c = 1 - c;
    return vec3((x2 * one_c + c) * vec.x() + (xy * one_c - zs) * vec.y() + (xz * one_c + ys) * vec.z(),
                (xy * one_c + zs) * vec.x() + (y2 * one_c + c) * vec.y() + (yz * one_c - xs) * vec.z(),
                (xz * one_c - ys) * vec.x() + (yz * one_c + xs) * vec.y() + (z2 * one_c + c) * vec.z());
}

vec3 gmath::scale(const vec3 &vec, const vec3 &factors)
{
    return vec3(vec.x() * factors.x(), vec.y() * factors.y(), vec.z() * factors.z());
}
std::vector<point> gmath::projectTriangle(const point &a, const point &b, const point &c)
{
    // Create mutable copies of the input points
    point a_copy = a;
    point b_copy = b;
    point c_copy = c;

    // Set the z-coordinate of each point to 0
    a_copy.set_z(0);
    b_copy.set_z(0);
    c_copy.set_z(0);

    // Return a vector containing the modified points
    return {a_copy, b_copy, c_copy};
}