#ifndef Cube_H
#define Cube_H
#include <array>
#include "point.h"

// ---------------------------------------------------------------------
// Cube: represented compactly as 4 points instead of 8.
//
//   points[0] = origin        (the "min" corner, closest to the grid origin)
//   points[1] = origin + (size, 0, 0)   -> defines the X extent
//   points[2] = origin + (0, size, 0)   -> defines the Y extent
//   points[3] = origin + (0, 0, size)   -> defines the Z extent
//
// Because every sub-Cube produced by the grid is axis-aligned and has
// equal edge length on all 3 axes, these 4 points are sufficient to
// fully reconstruct all 8 vertices, the center, and the size.
// ---------------------------------------------------------------------
class Cube
{
private:
    std::array<point, 4> m_points;

public:
    Cube() = default;

    Cube(const point &origin, double size)
    {
        m_points[0] = origin;
        m_points[1] = origin + point(size, 0.0, 0.0);
        m_points[2] = origin + point(0.0, size, 0.0);
        m_points[3] = origin + point(0.0, 0.0, size);
    }

    const std::array<point, 4> &Getpoints() const { return m_points; }

    point Origin() const { return m_points[0]; }

    double Size() const
    {
        // Distance from origin to the X-extent point == edge length.
        return m_points[1].get_x() - m_points[0].get_x();
    }

    point Center() const
    {
        double s = Size();
        return m_points[0] + point(s * 0.5, s * 0.5, s * 0.5);
    }

    // Reconstructs all 8 corners on demand (not stored, to keep Cube small).
    std::array<point, 8> GetAllVertices() const
    {
        const point &o = m_points[0];
        double s = Size();
        return {
            point(o.get_x(), o.get_y(), o.get_z()),
            point(o.get_x() + s, o.get_y(), o.get_z()),
            point(o.get_x(), o.get_y() + s, o.get_z()),
            point(o.get_x() + s, o.get_y() + s, o.get_z()),
            point(o.get_x(), o.get_y(), o.get_z() + s),
            point(o.get_x() + s, o.get_y(), o.get_z() + s),
            point(o.get_x(), o.get_y() + s, o.get_z() + s),
            point(o.get_x() + s, o.get_y() + s, o.get_z() + s)};
    }

    bool Contains(const point &p) const
    {
        const point &o = m_points[0];
        double s = Size();
        return p.get_x() >= o.get_x() && p.get_x() <= o.get_x() + s &&
               p.get_y() >= o.get_y() && p.get_y() <= o.get_y() + s &&
               p.get_z() >= o.get_z() && p.get_z() <= o.get_z() + s;
    }
};

#endif // Cube_H