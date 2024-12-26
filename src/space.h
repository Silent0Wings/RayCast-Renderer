#ifndef SPACE_H
#define SPACE_H

#include <future>
#include <vector>
#include <thread>
#include <iostream>
#include "object.h"
#include "camera.h"
#include "ppm.cpp"


using namespace std;

/**
 * @class space
 * @brief Represents a space in 3D.
 * The space class encapsulates a 3D space defined by a vector of objects and cameras.
 * It provides methods to add objects and cameras to the space, and to trigger the camera ray behavior.
 * it also allows the ability to trigger the camera ray behavior for rendering
 */
class space {
public:
    vector<object> obj;
    vector<camera> cameras;

    // Constructors and Destructor
    space(): obj(), cameras() {}
    space(vector<object> obj) : obj(obj) {}

    // Add an object to the space
    void addObject(const object& o) {
        obj.push_back(o);
    }

    // Add a camera to the space
    void addCamera(const camera& c) {
        cameras.push_back(c);
    }

    // Overload operator<< for prunsigned inting
    friend ostream& operator<<(ostream& os, const space& s) {
        os << "Objects in space:\n";
        for (const auto& o : s.obj) {
            os << o << "\n";
        }
        os << "Cameras in space:\n";
        for (const auto& c : s.cameras) {
            os << c << "\n";
        }
        return os;
    }

    // Overload operator==
    bool operator==(const space& other) const {
        if (obj.size() != other.obj.size() || cameras.size() != other.cameras.size()) {
            return false;
        }
        for (size_t i = 0; i < obj.size(); ++i) {
            if (obj[i] != other.obj[i]) {
                return false;
            }
        }
        for (size_t i = 0; i < cameras.size(); ++i) {
            if (cameras[i] != other.cameras[i]) {
                return false;
            }
        }
        return true;
    }

    // trigger the camera ray behavior
    void triggerCameraRay() {
        if (cameras.empty() || obj.empty()) {
            cout << "No cameras or objects to process." << endl;
            return;
        }

        for (auto& cam : cameras) {

            for (auto& o : obj) { 

                cam.cameraToImage(o);
               // cout << "Processing Camera #" << &cam - &cameras[0] << endl;
               // cout << "Processing Object #" << &o - &obj[0] << endl;
               // cout << "Camera Image: " << endl;
               // cout << cam.getimage() << endl;
                // Output file path

                // Render the image and save it to the file
            }
        }

        
    }

    
    void threadedCameraRay(std::vector<std::future<void>>& futures) {
        for (auto& cam : cameras) {
            size_t camIndex = &cam - &cameras[0];
            std::cout << "Thread N*= " << camIndex << " |started!" << std::endl;
            for (auto& o : obj) {
                // Launch asynchronous task for processing each camera-object pair
                futures.push_back(std::async(std::launch::async, [&cam, &o, &camIndex]() {
                    // Calculate thread identifier for logging

                    // Log thread start

                    // Clear screen (optional, might not work in all consoles)
                    std::cout << "\033[2J\033[H";

                    // Process the object with the camera
                    cam.cameraToImage(o);

                    // Save the rendered image to a file
                    ImageRenderer::renderToFile(cam.getimage(), "output" + std::to_string(camIndex) + ".ppm");

                    // Log thread end
                }));
            }
            std::cout << "Thread N*= " << camIndex << " |ended!" << std::endl;
        }
    }

    // output the imges 
    void saveImages()
    {
        if(cameras.size()==0)
        return;

        for (auto& cam : cameras) {
            ImageRenderer::renderToFile(cam.getimage(), "output" + to_string(&cam - &cameras[0]) + ".ppm");
        }
    }

    void saveImage(camera c)
    {
        ImageRenderer::renderToFile(c.getimage(), "output_" + to_string(chrono::high_resolution_clock::now().time_since_epoch().count()) + ".ppm");
    }
};

#endif // SPACE_H