#ifndef MESH_READER_H
#define MESH_READER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "point.h"

using namespace std;

struct Vec3
{
    double x = 0, y = 0, z = 0;
};

struct CameraData
{
    Vec3 position, rotation;
    double fov = 60.0, nearClip = 0.1, farClip = 1000.0;
    int resX = 800, resY = 800;
};

struct ObjectData
{
    int type = 0;
    Vec3 location, scale{1, 1, 1}, rotation;
};

class MeshReader
{
public:
    vector<vector<string>> verticesString;

    CameraData sceneCamera;
    bool hasCamera = false;
    vector<ObjectData> sceneObjects;

    MeshReader(string filename)
    {
        if (!filename.empty())
            loadMesh(filename, &verticesString);
    }

    MeshReader() {}

    point parseTuple(const string &s)
    {
        string clean;
        for (char c : s)
            if (c != '(' && c != ')')
                clean += c;
        stringstream ss(clean);
        string tok;
        double x, y, z;
        getline(ss, tok, ',');
        x = stod(tok);
        getline(ss, tok, ',');
        y = stod(tok);
        getline(ss, tok, ',');
        z = stod(tok);
        return point(x, y, z);
    }

    static Vec3 parseVec3(const string &s)
    {
        Vec3 v;
        string clean;
        for (char c : s)
            if (c != '(' && c != ')')
                clean += c;
        stringstream ss(clean);
        string tok;
        getline(ss, tok, ',');
        v.x = stod(tok);
        getline(ss, tok, ',');
        v.y = stod(tok);
        getline(ss, tok, ',');
        v.z = stod(tok);
        return v;
    }

    static vector<string> splitSemicolon(const string &line)
    {
        vector<string> parts;
        stringstream ss(line);
        string tok;
        while (getline(ss, tok, ';'))
            parts.push_back(tok);
        return parts;
    }

    bool loadScene(const string &path)
    {
        ifstream file(path);
        if (!file.is_open())
        {
            cerr << "Failed to open scene file: " << path << endl;
            return false;
        }

        hasCamera = false;
        sceneObjects.clear();

        string line;
        int lineNum = 0;
        while (getline(file, line))
        {
            lineNum++;
            if (line.empty())
                continue;
            auto parts = splitSemicolon(line);
            if (parts.empty())
                continue;

            if (parts[0] == "CAMERA" && parts.size() == 5)
            {
                sceneCamera.position = parseVec3(parts[1]);
                sceneCamera.rotation = parseVec3(parts[2]);
                Vec3 f = parseVec3(parts[3]);
                sceneCamera.fov = f.x;
                sceneCamera.nearClip = f.y;
                sceneCamera.farClip = f.z;

                Vec3 res = parseVec3(parts[4] + ", 0");
                sceneCamera.resX = (int)res.x;
                sceneCamera.resY = (int)res.y;

                hasCamera = true;
            }
            else if (parts[0] == "OBJECT" && parts.size() == 5)
            {
                ObjectData od;
                od.type = stoi(parts[1]);
                od.location = parseVec3(parts[2]);
                od.scale = parseVec3(parts[3]);
                od.rotation = parseVec3(parts[4]);
                sceneObjects.push_back(od);
            }
            else
            {
                cerr << "Warning: malformed scene line " << lineNum << ": " << line << endl;
            }
        }
        return true;
    }

    // Template methods: ObjectT/CameraT only need to be fully known at the
    // CALL SITE (wherever you #include object.h/camera.h AND MeshReader.h together).
    // MeshReader.h itself never includes object.h or camera.h.
    template <typename ObjectT>
    vector<ObjectT> toObjects() const
    {
        vector<ObjectT> result;
        result.reserve(sceneObjects.size());
        for (const auto &od : sceneObjects)
        {
            double avgScale = (od.scale.x + od.scale.y + od.scale.z) / 3.0;
            ObjectT obj((typename ObjectT::PrimitiveType)od.type, avgScale,
                        point(od.location.x, od.location.y, od.location.z));
            obj.setRotation(point(od.rotation.x, od.rotation.y, od.rotation.z));
            result.push_back(obj);
        }
        return result;
    }

    template <typename CameraT>
    CameraT toCamera() const
    {
        return CameraT(
            point(sceneCamera.position.x, sceneCamera.position.y, sceneCamera.position.z),
            point(sceneCamera.rotation.x, sceneCamera.rotation.y, sceneCamera.rotation.z),
            sceneCamera.fov, sceneCamera.nearClip, sceneCamera.farClip);
    }

    bool loadMesh(const std::string &filename, std::vector<std::vector<string>> *vertices)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "failed to open the file !" << endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> vertex;
            size_t pos = 0;
            while ((pos = line.find(';')) != std::string::npos)
            {
                vertex.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            vertex.push_back(line);
            vertices->push_back(vertex);
        }
        file.close();
        return true;
    }

    bool convertMesh(vector<vector<point>> *vertices)
    {
        if (verticesString.size() == 0)
        {
            cerr << "failed to open the file !" << endl;
            return false;
        }
        for (size_t i = 0; i < verticesString.size(); i++)
        {
            vector<point> v;
            for (size_t j = 0; j < verticesString[i].size(); j++)
                v.push_back(parseTuple(verticesString[i][j]));
            vertices->push_back(v);
        }
        return verticesString.size() == vertices->size();
    }

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif // MESH_READER_H