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

    void enableGrid(std::size_t divisions)
    {
        for (auto &o : obj)
        {
            o.enableGrid(divisions);
        }
    }

    // return the number of available threads on the system
    size_t getAvailableThreads()
    {
        size_t n = static_cast<size_t>(std::thread::hardware_concurrency());
        cout << "Available threads: " << n << endl;
        return n > 0 ? n : 4; // 4 just in case it fails
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

    // This launches a thread for each camera
    void launchThreadedCameraSplit()
    {

        // splits the camera
        size_t originalH = 0;
        size_t originalW = 0;
        if (cameras.size() == 1)
        {

            originalH = cameras.at(0).getheight();
            originalW = cameras.at(0).getwidth();
            vector<camera> cam_list = cameras.at(0).splitCamera(cameras.at(0), getAvailableThreads());
            cameras.clear();
            cameras = cam_list;
        }
        else
        {
            throw std::runtime_error("launchThreadedCameraSplit() is only supported for a single camera in the space.");
        }

        // launche the threads
        launchThreadedCamera();

        // stitch the images back together
        saveStitchedImage("stitched_output_", originalH, originalW);
    }

    // This launches a thread for each camera
    void launchThreadedCamera()
    {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::future<void>> futures;

        for (size_t camIndex = 0; camIndex < cameras.size(); ++camIndex)
        {
            futures.push_back(std::async(std::launch::async, [&, camIndex]()
                                         {
                for (auto& o : obj) {
                    cameras[camIndex].cameraToImage(o);
                } }));
        }

        for (auto &future : futures)
        {
            future.get();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "test() elapsed time: " << elapsed.count() << " ms\n";
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
            saveImage(cameras[i], "output" + to_string(i));
        }
    }
    void saveImage(camera c, string name = "output_")
    {
        if (name.empty() || name == "output_")
        {
            name = "output_" + to_string(chrono::high_resolution_clock::now().time_since_epoch().count());
        }
        ImageRenderer::renderToFile(c.getimage(), name + ".ppm");
    }
    void saveStitchedImage(string name = "stitched_output_", size_t originalH = 0, size_t originalW = 0)
    {
        if (originalH == 0 || originalW == 0)
        {
            throw std::runtime_error("Original height and width must be provided for stitching.");
        }

        if (name.empty() || name == "stitched_output_")
        {
            name = "stitched_output_" + to_string(chrono::high_resolution_clock::now().time_since_epoch().count());
        }
        // stitch the images back together
        image finalstitched = cameras.at(0).construct_split(cameras, originalH, originalW);
        ImageRenderer::renderToFile(finalstitched, "stitched.ppm");
    }

    // loading camera and objects from a scene file
    void loadFromFile(const string &path_to_scene)
    {
        // read the exported scene file and load the camera and objects into the space
        MeshReader reader;
        loadReader(path_to_scene, reader);

        // Load objects
        loadObjectFromFile(reader);

        // Load camera
        loadCameraFromFile(reader);
    }
    void loadReader(const string &path_to_scene, MeshReader &reader)
    {
        if (!reader.loadScene(path_to_scene))
        {
            cerr << "Failed to load scene: " << path_to_scene << endl;
            return;
        }
    }
    void loadCameraFromFile(const MeshReader &reader)
    {
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
            double m10 = cy * sz;
            double m11 = sx * sy * sz + cx * cz;
            double m20 = -sy;
            double m21 = sx * cy;

            vec3 Xdirection(m00, m10, m20);
            vec3 Ydirection(m01, m11, m21);

            double step = 0.01;

            double perspectiveScale = reader.sceneCamera.perspectiveScale > 1.0
                                          ? reader.sceneCamera.perspectiveScale * 2
                                          : 1.0;

            double fovRad = reader.sceneCamera.fov * gmath::pi / 180.0;
            double halfWidth = reader.sceneCamera.resX / 2.0;
            double perspectiveForce =
                ((perspectiveScale - 1.0) * step * halfWidth) / tan(fovRad / 2.0);

            // Compute the ray direction that the old factory used to calculate
            // internally from the right-hand rule:
            //   direction = normalize(cross(indexFinger, midleFinger)) * thumb
            // Here Ydirection was indexFinger (row/forward) and Xdirection was midleFinger (col/up).
            int thumbFinger = 1;
            vec3 camRayDir = gmath::normalize(gmath::cross(Ydirection, Xdirection)) * thumbFinger;

            // Unified constructor (replaces camera::perspectiveCamera)
            camera cam(
                reader.sceneCamera.resY,
                reader.sceneCamera.resX,
                step,
                origin,
                Xdirection, // xDir  (column / width axis)
                Ydirection, // yDir  (row / height axis)
                camRayDir,
                perspectiveScale,
                perspectiveForce);

            cam.recenterTo(origin);
            cout << "Camera origin: " << origin << endl;
            cout << "Camera getOrigin(): " << cam.getOrigin() << endl;
            addCamera(cam);
        }
    }
    void loadObjectFromFile(const MeshReader &reader)
    {

        // Load objects
        for (const auto &objData : reader.sceneObjects)
        {
            double avgScale = (objData.scale.x + objData.scale.y + objData.scale.z) / 3.0;

            double rx = objData.rotation.x * gmath::pi / 180.0;
            double ry = objData.rotation.y * gmath::pi / 180.0;
            double rz = objData.rotation.z * gmath::pi / 180.0;

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

            double traceVal = m00 + m11 + m22;
            double angleRad = acos(std::clamp((traceVal - 1.0) / 2.0, -1.0, 1.0));
            double angleDeg = angleRad * 180.0 / gmath::pi;

            vec3 axis(0, 0, 1);
            double sinAngle = sin(angleRad);
            if (sinAngle > 1e-6)
            {
                axis = vec3(
                    (m21 - m12) / (2.0 * sinAngle),
                    (m02 - m20) / (2.0 * sinAngle),
                    (m10 - m01) / (2.0 * sinAngle));
            }

            object obj(
                (primitive)objData.type,
                avgScale,
                point(objData.location.x, objData.location.y, objData.location.z),
                angleDeg,
                axis);
            addObject(obj);
        }
    }
};

#endif // SPACE_H