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

        for (auto& future : futures) {
            future.get();
        }
    }

    // This launches a thread for each camera
    void launchThreadedCamera() {
        std::vector<std::future<void>> futures;

        for (size_t camIndex = 0; camIndex < cameras.size(); ++camIndex) {
            futures.push_back(std::async(std::launch::async, [&, camIndex]() {
                for (auto& o : obj) {
                    cameras[camIndex].cameraToImageOptimized(o);
                }
            }));
        }

        for (auto& future : futures) {
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
};

#endif // SPACE_H