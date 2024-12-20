#ifndef IMAGERENDERER_H
#define IMAGERENDERER_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "image.h"

class ImageRenderer {
public:
    static void renderToFile(const image& img, const std::string& filePath) {
        // Open the file for writing
        std::ofstream outFile(filePath);
        if (!outFile) {
            std::cerr << "Error: Cannot open file " << filePath << " for writing.\n";
            return;
        }

        // Write the PPM header
        outFile << "P3\n" << img.getwidth() << ' ' << img.getheight() << "\n255\n";

      // Render the pixel data stored in the Image object
        for (unsigned int i = 0; i < img.getheight(); ++i) {  // Outer loop: height (rows)
            //std::clog << "\rScanlines remaining: " << (img.getheight() - i) << ' ' << std::flush;
            for (unsigned int j = 0; j < img.getwidth(); ++j) {  // Inner loop: width (columns)
                color pixel = img.get(i, j);  // Access pixel data using width (j) and height (i)
                int r = static_cast<int>(pixel.r());
                int g = static_cast<int>(pixel.g());
                int b = static_cast<int>(pixel.b());
                outFile << r << ' ' << g << ' ' << b << '\n';

            }
        }


        outFile.close();
        std::clog << "\rDone. Image saved to " << filePath << "                 \n";

        // File conversion using ImageMagick
        std::string pngFile = filePath;
        size_t pos = pngFile.find_last_of('.');
        if (pos != std::string::npos) {
            pngFile.replace(pos, pngFile.length() - pos, ".png");
        } else {
            pngFile += ".png";
        }

    
        // Convert PPM to PNG
        std::string convertCommand = "magick convert " + filePath + " " + pngFile;
        std::cout << "convertCommand: " << convertCommand << std::endl;
        int convertResult = system(convertCommand.c_str());
        if (convertResult != 0) {
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
        int openResult = system(openCommand.c_str());
        if (openResult != 0) {
            std::cerr << "Error: Failed to open " << pngFile << ".\n";
        }


        // Delete the PPM file
        std::string deleteCommand = "del " + filePath;
        int deleteResult = system(deleteCommand.c_str());
        if (deleteResult != 0) {
            std::cerr << "Error: Failed to delete " << filePath << ".\n";
        }
    }
};

#endif // IMAGERENDERER_H
