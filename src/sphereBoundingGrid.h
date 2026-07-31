#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <utility>
#include "point.h"
#include "Cube.h"

struct CubeData
{
    std::array<point, 4> bounds;
    point origin;
    std::vector<std::array<point, 3>> triples; // each entry is exactly 3 points
    std::array<std::size_t, 6> neighbors;      // [left, right, front, back, bottom, top]
};

struct CubeEntry
{
    Cube cube;
    CubeData data;
};

class sphereBoundingGrid
{
public:
    // Empty-grid constructor.
    sphereBoundingGrid(const point &sphereCenter, double sphereRadius, std::size_t divisions)
        : m_center(sphereCenter), m_radius(sphereRadius), m_divisions(divisions)
    {
        Validate();
        BuildBoundingCube();
        Subdivide(nullptr, nullptr);
    }

    // Returns the [min,max] cube index range along each axis that a triangle's AABB touches.
    bool TriangleCubeRange(const std::array<point, 3> &tri,
                           std::size_t &ixMin, std::size_t &ixMax,
                           std::size_t &iyMin, std::size_t &iyMax,
                           std::size_t &izMin, std::size_t &izMax) const
    {
        point minP = tri[0], maxP = tri[0];
        for (int k = 1; k < 3; ++k)
        {
            minP = point(std::min(minP.get_x(), tri[k].get_x()),
                         std::min(minP.get_y(), tri[k].get_y()),
                         std::min(minP.get_z(), tri[k].get_z()));
            maxP = point(std::max(maxP.get_x(), tri[k].get_x()),
                         std::max(maxP.get_y(), tri[k].get_y()),
                         std::max(maxP.get_z(), tri[k].get_z()));
        }

        const point origin = m_boundingCube.Origin();
        const double fullSize = m_boundingCube.Size();
        const double subSize = fullSize / static_cast<double>(m_divisions);

        auto clampIdx = [&](double d) -> std::size_t
        {
            double c = std::min(std::max(d, 0.0), fullSize - 1e-9);
            return static_cast<std::size_t>(std::min(c / subSize, static_cast<double>(m_divisions - 1)));
        };

        // If the triangle is entirely outside the bounding cube on any axis, reject.
        if (maxP.get_x() < origin.get_x() || minP.get_x() > origin.get_x() + fullSize)
            return false;
        if (maxP.get_y() < origin.get_y() || minP.get_y() > origin.get_y() + fullSize)
            return false;
        if (maxP.get_z() < origin.get_z() || minP.get_z() > origin.get_z() + fullSize)
            return false;

        ixMin = clampIdx(minP.get_x() - origin.get_x());
        ixMax = clampIdx(maxP.get_x() - origin.get_x());
        iyMin = clampIdx(minP.get_y() - origin.get_y());
        iyMax = clampIdx(maxP.get_y() - origin.get_y());
        izMin = clampIdx(minP.get_z() - origin.get_z());
        izMax = clampIdx(maxP.get_z() - origin.get_z());
        return true;
    }

    // Accelerated constructor: pass a vector of point-groups (no flattening).
    // For each group:
    //   - If all points in the group lie in the SAME cube AND group.size() >= 3,
    //     that cube gets ALL possible triples from that group (triangle list: every 3 points).
    //   - If the group's points span multiple cubes, or group.size() < 3,
    //     the group is ignored (no partial assignment, no flattening).
    sphereBoundingGrid(const point &sphereCenter, double sphereRadius, std::size_t divisions,
                       const std::vector<std::vector<point>> &vertexGroups)
        : m_center(sphereCenter), m_radius(sphereRadius), m_divisions(divisions)
    {
        Validate();
        BuildBoundingCube();

        std::unordered_map<std::size_t, std::vector<std::array<point, 3>>> cubeTriples;

        for (const auto &group : vertexGroups)
        {
            if (group.empty())
                continue;

            std::size_t firstIdx;
            if (!TryIndexForPoint(group[0], firstIdx))
                continue; // first point outside bounding cube -> ignore group

            for (std::size_t i = 0; i + 2 < group.size(); i += 3)
            {
                std::array<point, 3> triple = {group[i], group[i + 1], group[i + 2]};

                std::size_t ixMin, ixMax, iyMin, iyMax, izMin, izMax;
                if (!TriangleCubeRange(triple, ixMin, ixMax, iyMin, iyMax, izMin, izMax))
                    continue; // fully outside bounding cube

                for (std::size_t ix = ixMin; ix <= ixMax; ++ix)
                    for (std::size_t iy = iyMin; iy <= iyMax; ++iy)
                        for (std::size_t iz = izMin; iz <= izMax; ++iz)
                            cubeTriples[IndexOf(ix, iy, iz)].push_back(triple);
            }
        }

        SubdivideWithTriples(&cubeTriples, nullptr);
    }

    // Change these three functions:
    const std::unordered_map<std::size_t, CubeEntry> &getCubes() const { return m_entries; }

    const std::unordered_map<std::size_t, CubeEntry> &Entries() const { return m_entries; }
    std::unordered_map<std::size_t, CubeEntry> &Entries() { return m_entries; }

    // Fix FindEntry to work with map iteration (entry is now a pair<key, value>)
    CubeEntry *FindEntry(const std::function<bool(const CubeEntry &)> &predicate)
    {
        for (auto &kv : m_entries)
            if (predicate(kv.second))
                return &kv.second;
        return nullptr;
    }

    std::size_t Divisions() const { return m_divisions; }
    std::size_t SubCubeCount() const { return m_divisions * m_divisions * m_divisions; }

    std::size_t IndexOf(std::size_t ix, std::size_t iy, std::size_t iz) const
    {
        return ix * m_divisions * m_divisions + iy * m_divisions + iz;
    }

    bool RayGridRange(const point &ro, const point &rd,
                      double &tEnter, double &tExit) const
    {
        const point o = m_boundingCube.Origin();
        const double s = m_boundingCube.Size();

        const double ros[3] = {ro.get_x(), ro.get_y(), ro.get_z()};
        const double rds[3] = {rd.get_x(), rd.get_y(), rd.get_z()};
        const double lo[3] = {o.get_x(), o.get_y(), o.get_z()};

        tEnter = 0.0;
        tExit = std::numeric_limits<double>::infinity();

        for (int a = 0; a < 3; ++a)
        {
            const double hi = lo[a] + s;
            if (std::fabs(rds[a]) < 1e-12)
            {
                if (ros[a] < lo[a] || ros[a] > hi)
                    return false;
                continue;
            }
            double t1 = (lo[a] - ros[a]) / rds[a];
            double t2 = (hi - ros[a]) / rds[a];
            if (t1 > t2)
                std::swap(t1, t2);
            tEnter = std::max(tEnter, t1);
            tExit = std::min(tExit, t2);
            if (tEnter > tExit)
                return false;
        }
        return true;
    }

    std::vector<std::pair<std::size_t, double>> TraverseRay(const point &ro, const point &rd) const
    {
        std::vector<std::pair<std::size_t, double>> visitedCubes;

        double tEnter, tExit;
        if (!RayGridRange(ro, rd, tEnter, tExit))
            return visitedCubes;

        const point o = m_boundingCube.Origin();
        const double cell = m_boundingCube.Size() / static_cast<double>(m_divisions);
        const long n = static_cast<long>(m_divisions);
        const double inf = std::numeric_limits<double>::infinity();

        const double ros[3] = {ro.get_x(), ro.get_y(), ro.get_z()};
        const double rds[3] = {rd.get_x(), rd.get_y(), rd.get_z()};
        const double lo[3] = {o.get_x(), o.get_y(), o.get_z()};

        long idx[3], step[3];
        double tMax[3], tDelta[3];

        for (int a = 0; a < 3; ++a)
        {
            const double start = ros[a] + rds[a] * tEnter;
            long c = static_cast<long>(std::floor((start - lo[a]) / cell));
            c = std::min<long>(std::max<long>(c, 0), n - 1);
            idx[a] = c;

            if (rds[a] > 1e-12)
            {
                step[a] = 1;
                tMax[a] = tEnter + (lo[a] + (c + 1) * cell - start) / rds[a];
                tDelta[a] = cell / rds[a];
            }
            else if (rds[a] < -1e-12)
            {
                step[a] = -1;
                tMax[a] = tEnter + (lo[a] + c * cell - start) / rds[a];
                tDelta[a] = -cell / rds[a];
            }
            else
            {
                step[a] = 0;
                tMax[a] = inf;
                tDelta[a] = inf;
            }
        }

        double cubeDist = tEnter;
        for (;;)
        {
            visitedCubes.emplace_back(IndexOf(
                                          static_cast<std::size_t>(idx[0]),
                                          static_cast<std::size_t>(idx[1]),
                                          static_cast<std::size_t>(idx[2])),
                                      cubeDist);

            const int a = (tMax[0] < tMax[1])
                              ? (tMax[0] < tMax[2] ? 0 : 2)
                              : (tMax[1] < tMax[2] ? 1 : 2);

            if (step[a] == 0 || tMax[a] > tExit)
                break;

            idx[a] += step[a];
            if (idx[a] < 0 || idx[a] >= n)
                break;

            cubeDist = tMax[a];
            tMax[a] += tDelta[a];
        }

        return visitedCubes;
    }

    void PrecomputeNeighbors()
    {
        constexpr std::size_t kInvalid = static_cast<std::size_t>(-1);

        auto resolve = [&](bool inBounds, std::size_t candidate) -> std::size_t
        {
            if (!inBounds)
                return kInvalid;

            auto it = m_entries.find(candidate);
            if (it == m_entries.end() || it->second.data.triples.empty())
                return kInvalid; // skip empty/nonexistent neighbors

            return candidate;
        };

        for (std::size_t ix = 0; ix < m_divisions; ++ix)
            for (std::size_t iy = 0; iy < m_divisions; ++iy)
                for (std::size_t iz = 0; iz < m_divisions; ++iz)
                {
                    std::size_t idx = IndexOf(ix, iy, iz);
                    auto &entry = m_entries.at(idx);

                    entry.data.neighbors[0] = resolve(ix > 0, (ix > 0) ? IndexOf(ix - 1, iy, iz) : 0);
                    entry.data.neighbors[1] = resolve(ix < m_divisions - 1, (ix < m_divisions - 1) ? IndexOf(ix + 1, iy, iz) : 0);
                    entry.data.neighbors[2] = resolve(iy > 0, (iy > 0) ? IndexOf(ix, iy - 1, iz) : 0);
                    entry.data.neighbors[3] = resolve(iy < m_divisions - 1, (iy < m_divisions - 1) ? IndexOf(ix, iy + 1, iz) : 0);
                    entry.data.neighbors[4] = resolve(iz > 0, (iz > 0) ? IndexOf(ix, iy, iz - 1) : 0);
                    entry.data.neighbors[5] = resolve(iz < m_divisions - 1, (iz < m_divisions - 1) ? IndexOf(ix, iy, iz + 1) : 0);
                }
    }
    CubeEntry &At(std::size_t index) { return m_entries.at(index); }
    const CubeEntry &At(std::size_t index) const { return m_entries.at(index); }

    void SetTriples(std::size_t index, std::vector<std::array<point, 3>> triples)
    {
        m_entries.at(index).data.triples = std::move(triples);
    }

    void AddVertexTriple(std::size_t index, const point &p1, const point &p2, const point &p3)
    {
        m_entries.at(index).data.triples.push_back({p1, p2, p3});
    }

    void AddVertexTriple(std::size_t index, std::array<point, 3> triple)
    {
        m_entries.at(index).data.triples.push_back(triple);
    }

    std::size_t IndexForPoint(const point &p) const
    {
        std::size_t idx;
        if (!TryIndexForPoint(p, idx))
            throw std::out_of_range("Point lies outside the bounding cube.");
        return idx;
    }

    bool TryIndexForPoint(const point &p, std::size_t &outIndex) const
    {
        const point origin = m_boundingCube.Origin();
        const double fullSize = m_boundingCube.Size();
        const double subSize = fullSize / static_cast<double>(m_divisions);

        double dx = p.get_x() - origin.get_x();
        double dy = p.get_y() - origin.get_y();
        double dz = p.get_z() - origin.get_z();

        if (dx < 0.0 || dy < 0.0 || dz < 0.0 || dx > fullSize || dy > fullSize || dz > fullSize)
            return false;

        std::size_t ix = static_cast<std::size_t>(std::min(dx / subSize, static_cast<double>(m_divisions - 1)));
        std::size_t iy = static_cast<std::size_t>(std::min(dy / subSize, static_cast<double>(m_divisions - 1)));
        std::size_t iz = static_cast<std::size_t>(std::min(dz / subSize, static_cast<double>(m_divisions - 1)));

        outIndex = IndexOf(ix, iy, iz);
        return true;
    }

    std::vector<std::size_t> DistributeGroupsAsTriples(const std::vector<std::vector<point>> &vertexGroups,
                                                       std::vector<std::size_t> *outRejectedGroups = nullptr)
    {
        std::unordered_map<std::size_t, std::vector<std::array<point, 3>>> cubeTriples;

        for (const auto &group : vertexGroups)
        {
            if (group.empty())
            {
                if (outRejectedGroups)
                    outRejectedGroups->push_back(0);
                continue;
            }

            std::size_t firstIdx;
            if (!TryIndexForPoint(group[0], firstIdx))
            {
                if (outRejectedGroups)
                    outRejectedGroups->push_back(0);
                continue;
            }

            bool allSame = true;
            for (std::size_t i = 1; i < group.size(); ++i)
            {
                std::size_t idx;
                if (!TryIndexForPoint(group[i], idx) || idx != firstIdx)
                {
                    allSame = false;
                    break;
                }
            }

            if (!allSame)
            {
                if (outRejectedGroups)
                    outRejectedGroups->push_back(0);
                continue;
            }

            if (group.size() >= 3)
            {
                std::array<point, 3> triple = {
                    group[0],
                    group[1],
                    group[2]};
                cubeTriples[firstIdx].push_back(triple);
            }
            else
            {
                if (outRejectedGroups)
                    outRejectedGroups->push_back(0);
            }
        }

        std::vector<std::size_t> touchedIndices;
        touchedIndices.reserve(cubeTriples.size());

        for (auto &kv : cubeTriples)
        {
            for (const auto &triple : kv.second)
                AddVertexTriple(kv.first, triple);
            touchedIndices.push_back(kv.first);
        }

        return touchedIndices;
    }

private:
    point m_center;
    double m_radius;
    std::size_t m_divisions;

    Cube m_boundingCube;
    std::unordered_map<std::size_t, CubeEntry> m_entries;

    void Validate() const
    {
        if (m_radius <= 0.0)
            throw std::invalid_argument("Sphere radius must be positive.");
        if (m_divisions == 0)
            throw std::invalid_argument("Divisions (n) must be >= 1.");
    }

    void BuildBoundingCube()
    {
        point minCorner = m_center - point(m_radius, m_radius, m_radius);
        double side = m_radius * 2.0;
        m_boundingCube = Cube(minCorner, side);
    }

    void Subdivide(std::unordered_map<std::size_t, std::vector<std::array<point, 3>>> *prebucketedTriples,
                   std::vector<std::size_t> *outTouchedIndices)
    {
        m_entries.clear();

        const point origin = m_boundingCube.Origin();
        const double fullSize = m_boundingCube.Size();
        const double subSize = fullSize / static_cast<double>(m_divisions);

        for (std::size_t ix = 0; ix < m_divisions; ++ix)
        {
            for (std::size_t iy = 0; iy < m_divisions; ++iy)
            {
                for (std::size_t iz = 0; iz < m_divisions; ++iz)
                {
                    point subOrigin = origin + point(
                                                   subSize * static_cast<double>(ix),
                                                   subSize * static_cast<double>(iy),
                                                   subSize * static_cast<double>(iz));

                    Cube subCube(subOrigin, subSize);

                    CubeData data;
                    data.bounds = subCube.Getpoints();
                    data.origin = subCube.Origin();

                    std::size_t idx = IndexOf(ix, iy, iz);

                    if (prebucketedTriples)
                    {
                        auto it = prebucketedTriples->find(idx);
                        if (it != prebucketedTriples->end())
                        {
                            data.triples = std::move(it->second);
                            if (outTouchedIndices)
                                outTouchedIndices->push_back(idx);
                        }
                    }

                    // Always insert the cube (even if empty)
                    m_entries[idx] = CubeEntry{std::move(subCube), std::move(data)};
                }
            }
        }
    }

    // Convenience wrapper name used by the accelerated constructor.
    void SubdivideWithTriples(std::unordered_map<std::size_t, std::vector<std::array<point, 3>>> *prebucketedTriples,
                              std::vector<std::size_t> *outTouchedIndices)
    {
        Subdivide(prebucketedTriples, outTouchedIndices);
    }
};