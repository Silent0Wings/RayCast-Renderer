/**
 * @file MeshReader.h
 * @brief Defines the MeshReader class for reading text files to extract mesh data.
 */

#ifndef MESH_READER_H
#define MESH_READER_H

#include <string>
#include <vector>
#include <fstream>
 
#include "point.h"

using namespace std;
/**
 * @class MeshReader
 * @brief Represents a mesh reader for reading text files to extract mesh data.
 * The MeshReader reads a text file that has lines each line contains a set of vertices that 
 * represent a face each vector position is seperated by a semicolone and each coordinate is seperated by a comma.
 */
class MeshReader {
public:
    vector<vector<string>> verticesString;
    MeshReader(string filename)
    {
        if (!filename.empty())
        {
            loadMesh(filename,&verticesString);
        }
        
    }

    bool loadMesh(const std::string& filename , std::vector<std::vector<string>>* vertices)
    {
        // Open the file from filename
        std::ifstream file(filename);   
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            //cout << line << endl;
            std::vector<std::string> vertex;
            // split the line by comma , and we add them to the vertex vector
            size_t pos = 0;
            while ((pos = line.find(';')) != std::string::npos) {
                vertex.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            vertex.push_back(line); // Add the last element

                        
            vertices->push_back(vertex);
        }

        file.close();
        return true;

    }
    bool convertMesh(std::vector<std::vector<point>>* vertices)
    {

        if (verticesString.size()==0)
        {
            return false;
        }
        

        for (size_t i = 0; i < verticesString.size(); i++)
        {
            vector<point> v;
            for (size_t j = 0; j < verticesString[i].size(); j++)
            {
                point p(verticesString[i][j]);
                v.push_back(p);                
            }
            vertices->push_back(v);
            
        }

        if (verticesString.size()==vertices->size())
        {
            return true;
        }
        
    return false;
    }
    const std::vector<float>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    bool parseFile(const std::string& filename);
};

#endif // MESH_READER_H