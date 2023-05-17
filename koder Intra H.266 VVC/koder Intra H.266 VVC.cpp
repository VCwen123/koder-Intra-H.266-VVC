#include <iostream>
#include <vector>
#include <limits>
#include <opencv2/opencv.hpp>
#include "BlockPrediction.h"

// Funkcja do wczytywania bloku pikseli z obrazu
Block readBlockFromImage(const cv::Mat& image, int startX, int startY) {
    Block block;
    block.pixels.resize(BlockSize, std::vector<int>(BlockSize, 0));

    for (int i = 0; i < BlockSize; ++i) {
        for (int j = 0; j < BlockSize; ++j) {
            int pixelX = startX + j;
            int pixelY = startY + i;
            block.pixels[i][j] = static_cast<int>(image.at<uchar>(pixelY, pixelX));
        }
    }

    return block;
}

int main() {
    BlockPrediction bp;
    cv::Mat image = cv::imread("lenna.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Failed to load image." << std::endl;
        return 1;
    }

    int startX = 0; // Współrzędna x początku bloku
    int startY = 0; // Współrzędna y początku bloku

    Block block = readBlockFromImage(image, startX, startY);
    PredictedBlock predictedBlock = bp.analyzeBlock(block);

    std::cout << "Predicted mode: " << predictedBlock.mode << std::endl;
    std::cout << "Predicted pixels:" << std::endl;
    for (const auto& row : predictedBlock.pixels) {
        for (int pixel : row) {
            std::cout << pixel << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}