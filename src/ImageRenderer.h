#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "color.h"
#include "image.h"
#include <cstdlib> // For C++ programs
// OR
#include <stdlib.h> // For C programs

class ImageRenderer
{
public:
    static std::vector<std::vector<color>> readPPM(const std::string &filename, int &width, int &height)
    {
        std::ifstream file(filename);
        if (!file)
        {
            throw std::runtime_error("Could not open file");
        }

        // Read and validate the PPM format
        std::string format;
        file >> format;
        if (format != "P3")
        { // Only handling P3 (ASCII) format
            throw std::runtime_error("Unsupported PPM format");
        }

        // Read image dimensions and max color value
        file >> width >> height;
        int maxColor;
        file >> maxColor;

        // Initialize a 2D vector for the image
        std::vector<std::vector<color>> image(height, std::vector<color>(width));

        // Read pixel data
        int r, g, b;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (!(file >> r >> g >> b))
                { // Read RGB values
                    throw std::runtime_error("Invalid PPM pixel data");
                }
                image[i][j] = color(static_cast<double>(r), static_cast<double>(g), static_cast<double>(b)); // Store the pixel
            }
        }

        return image;
    }

    static void renderToFile(const image &img, const std::string filePath, bool open_image = true)
    {
        // Open the file for writing
        std::ofstream outFile(filePath);
        cerr << "filePath: " << filePath << endl;
        if (!outFile)
        {
            std::cerr << "Error: Cannot open file " << filePath << " for writing.\n";
            return;
        }

        // Write the PPM header
        outFile << "P3\n"
                << img.getwidth() << ' ' << img.getheight() << "\n255\n";

        // Render the color data stored in the Image object
        for (unsigned int i = 0; i < img.getheight(); ++i)
        { // Outer loop: height (rows)
            // std::clog << "\rScanlines remaining: " << (img.getheight() - i) << ' ' << std::flush;
            for (unsigned int j = 0; j < img.getwidth(); ++j)
            {                                // Inner loop: width (columns)
                color color = img.get(i, j); // Access color data using width (j) and height (i)
                int r = static_cast<int>(color.r());
                int g = static_cast<int>(color.g());
                int b = static_cast<int>(color.b());
                outFile << r << ' ' << g << ' ' << b << '\n';
            }
        }

        outFile.close();
        std::clog << "\rDone. Image saved to " << filePath << "                 \n";

        // File conversion using ImageMagick
        std::string pngFile = filePath;
        size_t pos = pngFile.find_last_of('.');
        if (pos != std::string::npos)
        {
            pngFile.replace(pos, pngFile.length() - pos, ".png");
        }
        else
        {
            pngFile += ".png";
        }

        // Convert PPM to PNG
        std::string convertCommand = "magick " + filePath + " " + pngFile;
        std::cout << "convertCommand: " << convertCommand << std::endl;
        int convertResult = system(convertCommand.c_str());
        if (convertResult != 0)
        {
            std::cerr << "Error: Failed to convert " << filePath << " to " << pngFile << ".\n";
            return;
        }

        std::cout << "Conversion successful: " << filePath << " -> " << pngFile << "\n";

// Open the PNG file
#ifdef _WIN32
        std::string openCommand = "start " + pngFile;
#elif __linux__
        std::string openCommand = "xdg-open " + pngFile;
#elif __APPLE__
        std::string openCommand = "open " + pngFile;
#else
        std::cerr << "Error: Opening images is not supported on this platform.\n";
        return;
#endif
        if (open_image == 1)
        {
            int openResult = system(openCommand.c_str());
            if (openResult != 0)
            {
                std::cerr << "Error: Failed to open " << pngFile << ".\n";
            }

#ifdef _WIN32
            std::string deleteCommand = "del " + filePath;
#else
            std::string deleteCommand = "rm " + filePath;
#endif
            int deleteResult = system(deleteCommand.c_str());
            if (deleteResult != 0)
            {
                std::cerr << "Error: Failed to delete " << filePath << ".\n";
            }
        }
    }
    static void renderToFilePPM(const image &img, const std::string filePath)
    {
        // Open the file for writing
        std::ofstream outFile(filePath);
        cerr << "filePath: " << filePath << endl;
        if (!outFile)
        {
            std::cerr << "Error: Cannot open file " << filePath << " for writing.\n";
            return;
        }

        // Write the PPM header
        outFile << "P3\n"
                << img.getwidth() << ' ' << img.getheight() << "\n255\n";

        // Render the color data stored in the Image object
        for (unsigned int i = 0; i < img.getheight(); ++i)
        { // Outer loop: height (rows)
            // std::clog << "\rScanlines remaining: " << (img.getheight() - i) << ' ' << std::flush;
            for (unsigned int j = 0; j < img.getwidth(); ++j)
            {                                // Inner loop: width (columns)
                color color = img.get(i, j); // Access color data using width (j) and height (i)
                int r = static_cast<int>(color.r());
                int g = static_cast<int>(color.g());
                int b = static_cast<int>(color.b());
                outFile << r << ' ' << g << ' ' << b << '\n';
            }
        }

        outFile.close();
        std::clog << "\rDone. Image saved to " << filePath << "                 \n";

        // File conversion using ImageMagick
        std::string pngFile = filePath;
        size_t pos = pngFile.find_last_of('.');
        if (pos != std::string::npos)
        {
            pngFile.replace(pos, pngFile.length() - pos, ".png");
        }
        else
        {
            pngFile += ".png";
        }
    }
};

#endif // IMAGERENDERER_H
