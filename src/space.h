#ifndef SPACE_H
#define SPACE_H

#include <future>
#include <vector>
#include <thread>
#include <iostream>
#include "object.h"
#include "camera.h"
#include "ppm.cpp"
#include "RayTrace.h"

using namespace std;

/**
 * @class space
 * @brief Represents a space in 3D.
 * The space class encapsulates a 3D space defined by a vector of objects and cameras.
 * It provides methods to add objects and cameras to the space, and to trigger the camera ray behavior.
 * it also allows the ability to trigger the camera ray behavior for rendering
 */
class space
{
public:
    vector<object> obj;
    vector<camera> cameras;

    // Constructors and Destructor
    space() : obj(), cameras() {}
    space(vector<object> temp_obj) : obj(temp_obj) {}

    // Add an object to the space
    void addObject(const object &o)
    {
        obj.push_back(o);
    }

    // Add a camera to the space
    void addCamera(const camera &c)
    {
        cameras.push_back(c);
    }

    // Overload operator<< for prunsigned inting
    friend ostream &operator<<(ostream &os, const space &s)
    {
        os << "Objects in space:\n";
        for (const auto &o : s.obj)
        {
            os << o << "\n";
        }
        os << "Cameras in space:\n";
        for (const auto &c : s.cameras)
        {
            os << c << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const space &other) const
    {
        if (obj.size() != other.obj.size() || cameras.size() != other.cameras.size())
        {
            return false;
        }
        for (size_t i = 0; i < obj.size(); ++i)
        {
            if (obj[i] != other.obj[i])
            {
                return false;
            }
        }
        for (size_t i = 0; i < cameras.size(); ++i)
        {
            if (cameras[i] != other.cameras[i])
            {
                return false;
            }
        }
        return true;
    }

    // trigger the camera ray behavior
    void triggerCameraRay()
    {
        if (cameras.empty() || obj.empty())
        {
            cout << "Empty Space :No cameras or objects to process." << endl;
            return;
        }
        for (auto &cam : cameras)
        {
            for (auto &o : obj)
            {
                cam.cameraToImage(o);
            }
        }
    }

    // trigger the camera ray behavior
    void triggerCameraRayOptimized()
    {

        if (cameras.empty() || obj.empty())
        {
            cout << "Empty Space :No cameras or objects to process." << endl;
            return;
        }
        for (auto &cam : cameras)
        {
            for (auto &o : obj)
            {
                cam.cameraToImageOptimized(o);
            }
        }
    }

    // trigger the camera ray behavior
    void triggerRayTrace(size_t bounce)
    {
        // for now it wil only support 1 camera

        vector<vector<RayTrace>> traceGrid;

        for (size_t i = 0; i < cameras.at(0).getwidth(); i++) // create a grid of raytracer rays of a size equivalent to the camera dimension
        {
            vector<RayTrace> tempRow;
            for (size_t j = 0; j < cameras.at(0).getheight(); j++)
            {
                RayTrace newRay = RayTrace(cameras.at(0).getGridRay().at(i).at(j));
                tempRow.push_back(newRay);
            }
            traceGrid.push_back(tempRow);
        }

        for (size_t i = 0; i < traceGrid.size(); i++) // trigger ray tracing one by one and assign a color in the image stored within the camera
        {
            for (size_t j = 0; j < traceGrid.at(i).size(); j++)
            {
                //    void trace(const size_t Bounce, const vector<object> *objects)

                traceGrid.at(i).at(j).trace(bounce, &obj);
                // cout << traceGrid.at(i).at(j).getPixelValue() << endl;
                cameras.at(0)
                    .setColor(i, j, traceGrid.at(i).at(j).getPixelValue());
            }
        }
    }

    // trigger async behavior
    void threadedCameraRay(std::vector<std::future<void>> &futures)
    {
        for (auto &cam : cameras)
        {
            // cout << "!!!!!!!!!!!!!!!!!" << endl;
            size_t camIndex = &cam - &cameras[0];
            // std::cout << "Thread N*= " << camIndex << " |started!" << std::endl;
            for (auto &o : obj)
            {
                // Launch asynchronous task for processing each camera-object pair
                futures.push_back(std::async(std::launch::async, [&cam, &o, camIndex]()
                                             {
                                                 // Calculate thread identifier for logging

                                                 // Log thread start

                                                 // Clear screen (optional, might not work in all consoles)

                                                 // Process the object with the camera
                                                 cam.cameraToImage(o);

                                                 // Save the rendered image to a file
                                                 // ImageRenderer::renderToFile(cam.getimage(), "output" + std::to_string(camIndex) + ".ppm");
                                                 // cerr << " camIndex= " << camIndex << "!" << endl;

                                                 // Log thread end
                                             }));
                // std::cout << "\033[2J\033[H";
            }
            // std::cout << "Thread N*= " << camIndex << " |ended!" << std::endl;
        }
    }
    void threadedCameraRayOptimized(std::vector<std::future<void>> &futures)
    {
        for (auto &cam : cameras)
        {
            // cout << "!!!!!!!!!!!!!!!!!" << endl;
            size_t camIndex = &cam - &cameras[0];
            // std::cout << "Thread N*= " << camIndex << " |started!" << std::endl;
            for (auto &o : obj)
            {
                // Launch asynchronous task for processing each camera-object pair
                futures.push_back(std::async(std::launch::async, [&cam, &o, camIndex]()
                                             {
                                                 // Calculate thread identifier for logging

                                                 // Log thread start

                                                 // Clear screen (optional, might not work in all consoles)

                                                 // Process the object with the camera
                                                 cam.cameraToImageOptimized(o);

                                                 // Save the rendered image to a file
                                                 // ImageRenderer::renderToFile(cam.getimage(), "output" + std::to_string(camIndex) + ".ppm");
                                                 // cerr << " camIndex= " << camIndex << "!" << endl;

                                                 // Log thread end
                                             }));
                // std::cout << "\033[2J\033[H";
            }
            // std::cout << "Thread N*= " << camIndex << " |ended!" << std::endl;
        }
    }

    // this laucnhes a thread for each camera to object pair : 10 objects & 10 cameras = 100 threads !!! not good
    void launchThreadedCameraRay()
    {
        std::vector<std::future<void>> futures;

        for (auto &cam : cameras)
        {
            // cout << "!!!!!!!!!!!!!!!!!" << endl;
            size_t camIndex = &cam - &cameras[0];
            // std::cout << "Thread N*= " << camIndex << " |started!" << std::endl;
            for (auto &o : obj)
            {
                // Launch asynchronous task for processing each camera-object pair
                futures.push_back(std::async(std::launch::async, [&cam, &o, camIndex]()
                                             {
                                                 // Calculate thread identifier for logging

                                                 // Log thread start

                                                 // Clear screen (optional, might not work in all consoles)

                                                 // Process the object with the camera
                                                 cam.cameraToImageOptimized(o);

                                                 // Save the rendered image to a file
                                                 // ImageRenderer::renderToFile(cam.getimage(), "output" + std::to_string(camIndex) + ".ppm");
                                                 // cerr << " camIndex= " << camIndex << "!" << endl;

                                                 // Log thread end
                                             }));
                // std::cout << "\033[2J\033[H";
            }
            // std::cout << "Thread N*= " << camIndex << " |ended!" << std::endl;
        }

        for (auto &future : futures)
        {
            future.get();
        }
    }

    // This launches a thread for each camera
    void launchThreadedCamera()
    {
        std::vector<std::future<void>> futures;

        for (size_t camIndex = 0; camIndex < cameras.size(); ++camIndex)
        {
            futures.push_back(std::async(std::launch::async, [&, camIndex]()
                                         {
                for (auto& o : obj) {
                    cameras[camIndex].cameraToImageOptimized(o);
                } }));
        }

        for (auto &future : futures)
        {
            future.get();
        }
    }

    // output the imges
    void saveImages()
    {
        if (cameras.size() == 0)
            return;
        for (size_t i = 0; i < cameras.size(); ++i)
        {
            cout << "_____________" << endl;
            cout << "Saving image " << to_string(i) << endl;
            ImageRenderer::renderToFile(cameras[i].getimage(), "output" + to_string(i) + ".ppm");
        }
    }

    void saveImage(camera c)
    {
        ImageRenderer::renderToFile(c.getimage(), "output_" + to_string(chrono::high_resolution_clock::now().time_since_epoch().count()) + ".ppm");
    }

    void saveImage(camera c, string name)
    {
        ImageRenderer::renderToFile(c.getimage(), "output_" + name + ".ppm");
    }

    void loadFromFile(const string &path_to_scene)
    {
        MeshReader reader;
        if (!reader.loadScene(path_to_scene))
        {
            cerr << "Failed to load scene: " << path_to_scene << endl;
            return;
        }

        // Load objects (rotation dropped — object class has no setRotation)
        for (const auto &objData : reader.sceneObjects)
        {
            double avgScale = (objData.scale.x + objData.scale.y + objData.scale.z) / 1;
            object obj((primitive)objData.type, avgScale, point(objData.location.x, objData.location.y, objData.location.z));
            addObject(obj);
        }

        // Load camera
        if (reader.hasCamera)
        {
            point origin(
                reader.sceneCamera.position.x,
                reader.sceneCamera.position.y,
                reader.sceneCamera.position.z);

            double rx = reader.sceneCamera.rotation.x * gmath::pi / 180.0;
            double ry = reader.sceneCamera.rotation.y * gmath::pi / 180.0;
            double rz = reader.sceneCamera.rotation.z * gmath::pi / 180.0;

            double cx = cos(rx), sx = sin(rx);
            double cy = cos(ry), sy = sin(ry);
            double cz = cos(rz), sz = sin(rz);

            double m00 = cy * cz;
            double m01 = sx * sy * cz - cx * sz;
            double m02 = cx * sy * cz + sx * sz;
            double m10 = cy * sz;
            double m11 = sx * sy * sz + cx * cz;
            double m12 = cx * sy * sz - sx * cz;
            double m20 = -sy;
            double m21 = sx * cy;
            double m22 = cx * cy;

            vec3 Xdirection(m00, m10, m20);
            vec3 Ydirection(m01, m11, m21);
            vec3 direction(-m02, -m12, -m22);

            int size = 800;
            double step = 0.01;

            camera cam(size, size, step, origin, Xdirection, Ydirection, direction);
            cam.recenterTo(origin); // fixes edge-vs-center mismatch
            addCamera(cam);
        }
    }
};

#endif // SPACE_H