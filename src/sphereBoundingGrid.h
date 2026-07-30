#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include "point.h"
#include "Cube.h"

struct CubeData
{
    std::array<point, 4> bounds;
    point origin;
    std::vector<std::array<point, 3>> triples; // each entry is exactly 3 points
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

    // Accelerated constructor: pass a vector of point-groups (no flattening).
    // For each group:
    //   - If all points in the group lie in the SAME cube AND group.size() >= 3,
    //     that cube gets one triple from the first 3 points of that group.
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
                continue; // group spans multiple cubes -> ignore

            if (group.size() >= 3)
            {
                std::array<point, 3> triple = {
                    group[0],
                    group[1],
                    group[2]};
                cubeTriples[firstIdx].push_back(triple);
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
        // m_entries is now a map, so no resize needed

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

                    // Only insert if this cube has geometry
                    if (!data.triples.empty())
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