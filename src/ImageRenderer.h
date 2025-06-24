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

    static void WriteBMP(const image &img, const std::string &filename)
    {
        int width = img.getwidth();
        int height = img.getheight();
        int rowSize = width * 3;
        int padding = (4 - (rowSize % 4)) % 4;
        int dataSize = (rowSize + padding) * height;
        int fileSize = 14 + 40 + dataSize;

        std::ofstream file(filename, std::ios::binary);
        if (!file)
        {
            std::cerr << "Failed to open file for writing.\n";
            return;
        }

        // BMP Header
        file.put('B');
        file.put('M');
        file.write(reinterpret_cast<char *>(&fileSize), 4);
        file.write("\0\0\0\0", 4);
        int dataOffset = 14 + 40;
        file.write(reinterpret_cast<char *>(&dataOffset), 4);

        // DIB Header
        int headerSize = 40;
        file.write(reinterpret_cast<char *>(&headerSize), 4);
        file.write(reinterpret_cast<char *>(&width), 4);
        file.write(reinterpret_cast<char *>(&height), 4);
        uint16_t planes = 1, bpp = 24;
        file.write(reinterpret_cast<char *>(&planes), 2);
        file.write(reinterpret_cast<char *>(&bpp), 2);
        int compression = 0;
        file.write(reinterpret_cast<char *>(&compression), 4);
        file.write(reinterpret_cast<char *>(&dataSize), 4);
        int res = 0;
        file.write(reinterpret_cast<char *>(&res), 4); // X ppm
        file.write(reinterpret_cast<char *>(&res), 4); // Y ppm
        file.write(reinterpret_cast<char *>(&res), 4); // Colors used
        file.write(reinterpret_cast<char *>(&res), 4); // Important colors

        // Pixel data (top-down)
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                color c = img.get(x, y);
                uint8_t r = static_cast<uint8_t>(c.r());
                uint8_t g = static_cast<uint8_t>(c.g());
                uint8_t b = static_cast<uint8_t>(c.b());
                file.put(b);
                file.put(g);
                file.put(r);
            }
            for (int i = 0; i < padding; i++)
                file.put(0);
        }

        file.close();
    }

    static image ReadBMP(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
        {
            std::cerr << "Failed to open file for reading.\n";
            return image();
        }

        // Skip BMP Header
        file.ignore(10);
        int dataOffset;
        file.read(reinterpret_cast<char *>(&dataOffset), 4);

        // DIB Header
        int headerSize;
        file.read(reinterpret_cast<char *>(&headerSize), 4);
        int width, height;
        file.read(reinterpret_cast<char *>(&width), 4);
        file.read(reinterpret_cast<char *>(&height), 4);
        uint16_t planes, bpp;
        file.read(reinterpret_cast<char *>(&planes), 2);
        file.read(reinterpret_cast<char *>(&bpp), 2);

        if (bpp != 24)
        {
            std::cerr << "Only 24-bit BMP files are supported.\n";
            return image();
        }

        // Skip the rest of the DIB header
        file.ignore(headerSize - 16);

        // Setup image
        image img(height, width); // matches your writer's (h, w)
        int rowSize = width * 3;
        int padding = (4 - (rowSize % 4)) % 4;

        // Read pixels top-down (as written)
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                uint8_t b = file.get();
                uint8_t g = file.get();
                uint8_t r = file.get();
                img.set(x, y, color(r, g, b));
            }
            file.ignore(padding);
        }

        file.close();
        return img;
    }
};

#endif // IMAGERENDERER_H
