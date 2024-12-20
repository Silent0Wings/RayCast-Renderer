#ifndef SPACE_H
#define SPACE_H

#include <vector>
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
    std::vector<object> obj;
    std::vector<camera> cameras;

    // Constructors and Destructor
    space(): obj(), cameras() {}
    space(std::vector<object> obj) : obj(obj) {}

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
            std::cout << "No cameras or objects to process." << std::endl;
            return;
        }

        for (auto& cam : cameras) {

            for (auto& o : obj) { 
                cam.cameraToImage(o);

               // std::cout << "Processing Camera #" << &cam - &cameras[0] << std::endl;
               // std::cout << "Processing Object #" << &o - &obj[0] << std::endl;
               // cout << "Camera Image: " << endl;
               // cout << cam.getimage() << endl;
                // Output file path

                // Render the image and save it to the file
                ImageRenderer::renderToFile(cam.getimage(), "output" + std::to_string(&cam - &cameras[0]) + ".ppm");
            }
        }
    }
};

#endif // SPACE_H