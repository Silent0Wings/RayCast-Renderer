#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

svoid vectorToImage(const std::vector<std::vector<cv::Vec3b>>& pixelData, const std::string& outputPath) {
    // Get the dimensions of the 2D vector
    int rows = pixelData.size();
    int cols = pixelData[0].size();

    // Create an empty OpenCV image (CV_8UC3 for 3-channel RGB image)
    cv::Mat image(rows, cols, CV_8UC3);

    // Fill the image with the data from the 2D vector
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image.at<cv::Vec3b>(i, j) = pixelData[i][j];
        }
    }

    // Save the image to the specified file
    cv::imwrite(outputPath, image);

    // Display the image
    cv::imshow("Generated Image", image);
    cv::waitKey(0);
}

int main() {
    // Example: Create a 2D vector with RGB values
    int rows = 100;
    int cols = 200;

    std::vector<std::vector<cv::Vec3b>> pixelData(rows, std::vector<cv::Vec3b>(cols));

    // Fill the vector with some gradient data
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Create a gradient of colors
            pixelData[i][j] = cv::Vec3b(i % 256, j % 256, (i + j) % 256); // (Blue, Green, Red)
        }
    }

    // Convert the 2D vector to an image and save it
    vectorToImage(pixelData, "output_image.png");

    return 0;
}
