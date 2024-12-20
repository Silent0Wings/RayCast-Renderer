#include "ImageRenderer.h"



int main1() {
    // Define an Image object
    image img(256, 256);

    // Output file path
    std::string outputPath = "output.ppm";

    // Render the image and save it to the file
    ImageRenderer::renderToFile(img, outputPath);

    return 0;
}